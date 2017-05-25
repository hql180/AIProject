#include "Agent.h"
#include "Action.h"
#include <set>
#include <algorithm>
#include "Gizmos.h"
#include <iostream>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

std::random_device rd1;
std::mt19937 gen1(rd1());
std::uniform_int_distribution<int> dis1(0, 40);

Agent::Agent() : m_currentTarget(nullptr), m_currentAction(nullptr)
{
}

Agent::Agent(glm::vec3& pos, PathGraph* graph, std::vector<Obstacle>* obstacles, std::vector<Fountain>* fountains, glm::vec4& colour, float FOV, float visionRange, float radius, glm::vec3& forwardDir)
	: m_postion(pos), m_velocity(0), m_dir(forwardDir), m_colour(colour)
{
	player = false;

	m_pathGraph = graph;

	m_obstacles = obstacles;

	m_fountains = fountains;
	
	m_radius = radius;

	m_visionRange = visionRange;

	m_FOV = (FOV / 2.f * M_PI) / 180.f;

	m_isDead = false;

	m_showDamage = 0;

	m_currentTarget = nullptr;

	m_currentAction = nullptr;
}

Agent::~Agent()
{
}

void Agent::setUp(Stats & stats, float moveSpeed, float preferedRange)
{
	m_stats = stats;

	m_moveSpeed = moveSpeed + 0.25f * m_stats.agility + 0.05 * m_stats.strength;

	m_maxHealth = m_stats.strength * 4.f + m_stats.vitality * 10.f;
	
	m_health = m_maxHealth;

	m_maxMana  = m_stats.intelligence * 10.f;

	m_mana = m_maxMana;

	m_preferedRange = preferedRange;
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

	glm::vec3 dirToTarget = glm::normalize(target->getPostion() - m_postion);
	glm::vec3 sideWays = glm::vec3(dirToTarget.z, 0, -dirToTarget.x);
	sideWays = glm::normalize(sideWays); // sideways horizontal unit vector

	for (auto& obs : *m_obstacles)
	{
		glm::vec3 vecToObs = obs.position - m_postion;
		float forwardDistance = glm::dot(dirToTarget, vecToObs);
		if (forwardDistance > 0 && forwardDistance < m_visionRange)
		{
			float sidewaysDistance = fabs(glm::dot(sideWays, vecToObs));

			if (sidewaysDistance <= obs.radius)
			{
				return false;
			}
		}
	}	
	return true;
}

bool Agent::inFOV(float angle)
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
		if(inLineOfSight(target))
		{
			m_evaluationTarget = target;
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
			m_currentActionScore *= 1.2f;
			if (m_currentAction)
				m_currentAction->exit(this, dt);
			bestAction->enter(this, dt);
		}
		return bestAction;
	}

	return m_currentAction;
}

Action * Agent::getCurrentAction()
{
	return m_currentAction;
}

void Agent::update(std::vector<Agent*> agentList, float dt)
{
	m_allAgents = agentList;

	if (glm::length(m_velocity) != 0)
	{
		m_dir = glm::normalize(m_velocity);
	}

	if (m_showDamage > 0)
	{
		aie::Gizmos::addRing(m_postion, m_radius * 0.5f, m_radius * 1.5f, 10, glm::vec4(.9, .1, .1, 1));
		m_showDamage -= dt;
	}

	if (m_health <= 0)
		respawn(this, dt);
	// loops through all other agents
	for (auto& agent : agentList)
	{
		if (agent != this && inVisionRange(agent) && inFOV(getAngleToTarget(agent)) && inLineOfSight(agent))
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

	if (player)
	{
		glm::vec3 vision = m_dir * m_visionRange;
		aie::Gizmos::addLine(m_postion, vision + m_postion, m_colour);
		aie::Gizmos::addLine(m_postion, glm::vec3(vision.x * cosf(m_FOV) + vision.z * sinf(m_FOV), 0, -vision.x * sinf(m_FOV) + vision.z * cosf(m_FOV)) + m_postion, m_colour);
		aie::Gizmos::addLine(m_postion, glm::vec3(vision.x * cosf(-m_FOV) + vision.z * sinf(-m_FOV), 0, -vision.x * sinf(-m_FOV) + vision.z * cosf(-m_FOV)) + m_postion, m_colour);

	}

	aie::Gizmos::addAABB(m_postion, glm::vec3(m_radius), m_colour);
	
	m_currentAction = getBestAction(dt);

	for (auto& action : m_targetedActions)
	{
		action->updateTimer(dt);
	}

	for (auto& action : m_actions)
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

void Agent::addHealth(float amount)
{
	m_health += amount;
}

float Agent::getAttackDamage()
{
	return m_stats.agility * 0.33f + m_stats.intelligence * 0.5f + m_stats.strength * 0.5f;
}

void Agent::subMana(float amount)
{
	m_mana -= amount;
}

void Agent::addMana(float amount)
{
	m_mana += amount;
}

float Agent::getCurrentMana()
{
	return m_mana;
}

float Agent::getManaPercentage()
{
	return m_mana/m_maxMana;
}

// Calculates angle from agent's forward velocity to target using dot product
float Agent::getAngleToTarget(Agent * target)
{
	return acosf(glm::dot(m_dir, glm::normalize(target->getPostion() - m_postion)));
}


float Agent::getMoveSpeed()
{
	return m_moveSpeed;
}

void Agent::setMoveSpeed(float speed)
{
	m_moveSpeed = speed;
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

void Agent::respawn(Agent* agent, float dt)
{
	m_health = m_maxHealth;
	m_mana = m_maxMana;
	if (m_currentAction)
		m_currentAction->exit(agent, dt);
	m_postion = glm::vec3(dis1(gen1), 0, dis1(gen1));
}

bool Agent::takeDamage(float damage)
{
	m_showDamage = 0.1f;
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

glm::vec3 Agent::getDirectionToTarget(Agent * from, Agent * to)
{
	return glm::normalize(to->getPostion() - from->getPostion());
}

glm::vec3 Agent::getForwardDir()
{
	return m_dir;
}

Stats & Agent::getStats()
{
	return m_stats;
}

float Agent::getRadius()
{
	return m_radius;
}

float Agent::getVisionRange()
{
	return m_visionRange;
}

float Agent::getPreferedRange()
{
	return m_preferedRange;
}

void Agent::setObstacle(std::vector<Obstacle>* obs)
{
	m_obstacles = obs;
}

std::vector<Obstacle>* Agent::getObstacle()
{
	return m_obstacles;
}

void Agent::setFountains(std::vector<Fountain>* fountains)
{
	m_fountains = fountains;
}

std::vector<Fountain>* Agent::getFountains()
{
	return m_fountains;
}

int Agent::getStatPoints()
{
	return m_statPoints;
}

void Agent::setStatPoints(int value)
{
	m_statPoints = value;
}

void Agent::updateStats()
{
	setUp(m_stats, m_moveSpeed, m_preferedRange);
}

Agent * Agent::getETarget()
{
	return m_evaluationTarget;
}

