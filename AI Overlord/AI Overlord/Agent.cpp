#include "Agent.h"
#include "Action.h"
#include <set>
#include <algorithm>

Agent::Agent()
{
}


Agent::~Agent()
{
}

glm::vec3 Agent::getPostion()
{
	return m_postion;
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

Action* Agent::getBestAction()
{
	Action* bestAction = nullptr;
	float bestScore;
	
	// First loop through selfactions and evaluate score
	for (auto& action : m_actions)
	{
		action.evaluate(&(*this));
		float score = 0;
		if (score > bestScore)
		{
			// set best action
			bestScore = score;
		}
	}

	// Then loop through actionable targets and evaluate available actions against that target
	for (auto& target : m_actionableHostiles)
	{
		for (auto& action : target->m_hostileActions)
		{
			action.evaluate(&(*this));
			float score = 0;
			if (score > bestScore)
			{
				// set best action
				bestScore = score;
			}
		}		
	}

	if (bestScore > m_currentActionScore || m_currentAction == nullptr)
	{
		if (bestAction != m_currentAction)
		{
			m_currentActionScore = bestScore;
			m_currentActionScore *= 1.05f;
			return bestAction;
		}
		else
		{
			return m_currentAction;
		}
	}

	return nullptr;
}

void Agent::update(std::vector<Agent*> agentList)
{
	// loops through all other agents
	for (auto& agent : agentList)
	{
		if (agent != this && inVisionRange(agent))
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


	m_currentAction = getBestAction();

	if (m_currentAction)
	{
		// update/perform action
	}
}
