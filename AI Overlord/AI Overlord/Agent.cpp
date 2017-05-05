#include "Agent.h"
#include "Action.h"
#include <set>
#include <algorithm>

Agent::Agent() : m_currentTarget(nullptr), m_currentAction(nullptr), m_actionCD(0.5f)
{
}

Agent::Agent(glm::vec3 & pos, PathGraph* graph, glm::vec4& colour) : m_postion(pos), m_pathGraph(graph), m_colour(colour), m_velocity(0), m_currentTarget(nullptr), m_currentAction(nullptr), m_actionCD(0.5f), m_moveSpeed(5.f)
{

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

	// Then loop through actionable targets and evaluate available actions against that target
	for (auto& target : m_actionableHostiles)
	{
		for (auto& action : target->m_hostileActions)
		{			
			float score = action->evaluate(this, dt);
			if (score > bestScore)
			{
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
	// loops through all other agents
	for (auto& agent : agentList)
	{
		if (agent != this) //  && inVisionRange(agent)
		{
			if (!contains(m_actionableHostiles, agent))
			{
				m_actionableHostiles.push_back(agent);
			}
		}
	}	

	for (int i = m_actionableHostiles.size() - 1; i > 0; --i)
	{
		if (!inVisionRange(m_actionableHostiles[i]))
		{
			/*auto it = m_actionableTargets.begin();
			std::advance(it, i);*/
			m_actionableHostiles.erase(m_actionableHostiles.begin() + i);
		}
	}

	m_currentAction = getBestAction(dt);

	for (auto& attack : m_targetedActions)
	{
		attack->updateTimer(dt);
	}

	//m_velocity = m_velocity * (0.80f);
	
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
	return 1.0f;
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

void Agent::setAttackTarget(Agent * agent)
{
	m_currentTarget = agent;
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

Agent * Agent::getAttackTarget()
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

glm::vec4 & Agent::getColour()
{
	return m_colour;
}

