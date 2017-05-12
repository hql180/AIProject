#include "Agent.h"
#include "Action.h"
#include <set>
#include <algorithm>
#include "Gizmos.h"
#include <iostream>
#include <random>

std::random_device rd1;
std::mt19937 gen1(rd1());
std::uniform_int_distribution<int> dis1(0, 40);

Agent::Agent() : m_currentTarget(nullptr), m_currentAction(nullptr), m_actionCD(0.5f)
{
}

Agent::Agent(glm::vec3 & pos, PathGraph* graph, glm::vec4& colour) : m_postion(pos), m_pathGraph(graph), m_colour(colour), m_velocity(0), m_currentTarget(nullptr), m_currentAction(nullptr), m_actionCD(0.5f), m_moveSpeed(5.f)
{
	m_FOV = 0.698132f;
	m_visionRange = 10.f;
	m_radius = 0.5f;

	m_maxMana = m_stats.intelligence * 10.f;
	m_mana = m_maxMana;

	m_maxHealth = m_stats.strength * 2.f + m_stats.vitality * 4.f;
	m_health = m_maxHealth;
}

Agent::~Agent()
{
}

glm::vec3 Agent::getPostion()
{
	return m_postion;
}

void Agent::setPosition(glm::vec3& position)
{
	m_postion = position;
}

bool Agent::contains(std::vector<Agent*> list, Agent * agent)
{
	if (std::find(list.begin(), list.end(), agent) != list.end())
		return true;
	return false;
}

bool Agent::inVisionRange(Agent * agent)
{
	if (glm::length(agent->getPostion() - m_postion) <= m_visionRange)
		return true;
	return false;
}

bool Agent::inLineOfSight(Agent * target)
{
	return (getAngleToTarget(target) <= m_FOV);
}

bool Agent::inLineOfSight(float angle)
{
	return (angle <= m_FOV);
}

Action* Agent::getBestAction(float dt)
{
	if (m_currentAction == nullptr)
	{
		m_currentActionScore = 0;
	}

	Action* bestAction = nullptr;
	float bestScore = 0;
	
	// First loop through selfactions and evaluate score
	for (auto& action : m_actions)
	{		
		float score = action->evaluate(this, dt);
		if (score > bestScore)
		{
			bestAction = action;
			bestScore = score;
		}
	}

	Agent* bestTarget = nullptr;

	// Then loop through actionable targets and evaluate available actions against that target
	for (auto& target : m_actionableHostiles)
	{
		m_currentTarget = target;
		for (auto& action : m_targetedActions)
		{			
			float score = action->evaluate(this, dt);
			if (score > bestScore)
			{
				bestTarget = target;
				bestAction = action;
				bestScore = score;
			}
		}		
	}

	if (bestScore > m_currentActionScore)
	{
		if (bestAction != m_currentAction)
		{
			if (m_currentAction != nullptr)
			{				
				m_currentAction->exit(this, dt);
			}
			m_currentTarget = bestTarget;
			m_currentActionScore = bestScore;
			m_currentActionScore *= 1.05f;
			bestAction->enter(this, dt);
		}
		return bestAction;
	}

	return m_currentAction;
}

void Agent::update(std::vector<Agent*> agentList, float dt)
{
	m_allAgents = agentList;

	if (m_health <= 0)
		respawn();
	// loops through all other agents
	for (auto& agent : agentList)
	{
		if (agent != this && inVisionRange(agent) && inLineOfSight(agent))
		{
			if (!contains(m_actionableHostiles, agent))
			{
				m_actionableHostiles.push_back(agent);
			}
		}
	}	

	for (int i = m_actionableHostiles.size() - 1; i >= 0; --i)
	{
		if (!inVisionRange(m_actionableHostiles[i]))
		{
			if (m_actionableHostiles[i] == m_currentTarget)
				m_currentTarget == nullptr;
			m_actionableHostiles.erase(m_actionableHostiles.begin() + i);
		}
	}
	glm::vec3 vision = glm::normalize(m_velocity)*m_visionRange;
	//aie::Gizmos::addArcRing(m_postion, 0, m_visionRange * 0.95f, m_visionRange, 4, 20, m_colour);
	aie::Gizmos::addLine(m_postion, vision + m_postion, m_colour);
	aie::Gizmos::addLine(m_postion, glm::vec3(vision.x * cosf(m_FOV) + vision.z * sinf(m_FOV), 0, -vision.x * sinf(m_FOV) + vision.z * cosf(m_FOV)) + m_postion, m_colour);
	aie::Gizmos::addLine(m_postion, glm::vec3(vision.x * cosf(-m_FOV) + vision.z * sinf(-m_FOV), 0, -vision.x * sinf(-m_FOV) + vision.z * cosf(-m_FOV)) + m_postion, m_colour);

	m_currentAction = getBestAction(dt);

	for (auto& action : m_targetedActions)
	{
		action->updateTimer(dt);
	}
	
	if (m_currentAction)
	{
		m_currentAction->updateAction(this, dt);
	}
}

float Agent::getCurrentHealth()
{
	return m_health;
}

float Agent::getHealthPercentage()
{
	return m_health/(m_maxHealth);
}

float Agent::getAttackDamage()
{
	return m_stats.agility * 0.5f + m_stats.intelligence + m_stats.strength;
}

void Agent::subMana(float amount)
{
	m_mana -= amount;
}

float Agent::getCurrentMana()
{
	return m_mana;
}

float Agent::getManaPercentage()
{
	return m_mana/m_maxMana;
}

float Agent::getDistanceToTarget()
{
	return m_distanceToTarget;
}

void Agent::checkDistanceToTarget()
{
	if (m_currentTarget)
	{
		m_distanceToTarget = glm::length(m_postion - m_currentTarget->getPostion());
	}
}

// Calculates angle from agent's forward velocity to target using dot product
float Agent::getAngleToTarget(Agent * target)
{
	return acosf(glm::dot(glm::normalize(m_velocity), glm::normalize(target->getPostion() - m_postion)));
}


float Agent::getMoveSpeed()
{
	return m_moveSpeed;
}

void Agent::updateMovement(float dt)
{
	if (glm::length(m_velocity) > m_moveSpeed)
		m_velocity = glm::normalize(m_velocity) * m_moveSpeed;
	m_postion = m_postion + m_velocity * dt;
}

void Agent::setCurrentAction(Action * action)
{
	m_currentAction = action;
}

void Agent::setTarget(Agent * agent)
{
	m_currentTarget = agent;
}

void Agent::respawn()
{
	m_health = m_maxHealth;
	m_mana = m_maxMana;
	m_currentAction = nullptr;
	m_postion = glm::vec3(dis1(gen1), 0, dis1(gen1));
}

bool Agent::takeDamage(float damage)
{
	m_health -= damage;
	return (m_health <= 0) ? true : false;
}

glm::vec3 Agent::getVelocity()
{
	return m_velocity;
}

void Agent::setVelocity(glm::vec3& targetVelocity)
{
	m_velocity = targetVelocity;
}

Agent * Agent::getTarget()
{
	return m_currentTarget;
}

Action * Agent::getEngage()
{
	return m_hostileActions.front();
}

PathGraph * Agent::getPathGraph()
{
	return m_pathGraph;
}

std::vector<Action*>& Agent::getHActions()
{
	return m_hostileActions;
}

std::vector<Action*>& Agent::getTActions()
{
	return m_targetedActions;
}

std::vector<Action*>& Agent::getActions()
{
	return m_actions;
}

std::vector<Agent*>& Agent::getHostiles()
{
	return m_actionableHostiles;
}

std::vector<Agent*>& Agent::getAllAgents()
{
	return m_allAgents;
}

glm::vec4 & Agent::getColour()
{
	return m_colour;
}

glm::vec3 Agent::getDirectionToTarget()
{
	if (m_currentTarget)
		return glm::normalize(m_currentTarget->getPostion() - m_postion);
	else
		return glm::vec3(0);
}

float Agent::getRadius()
{
	return m_radius;
}

