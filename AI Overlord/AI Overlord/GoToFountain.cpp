#include "GoToFountain.h"
#include "Agent.h"
#include "Gizmos.h"

GoToFountain::GoToFountain()
{
	name = " ";
}


GoToFountain::~GoToFountain()
{
}

float GoToFountain::evaluate(Agent * agent, float dt)
{
	if(agent->getHealthPercentage() < .45f || agent->getManaPercentage() < .45f)
	{
		float score = 0.5f - (0.5f *  (1.f - agent->getHealthPercentage() + 0.01f)) + 0.5f - (0.5f *  (1.f - agent->getManaPercentage() + 0.01f));

		if (agent->getManaPercentage() < 0.1f)
		{
			score += 1.f;
		}

		return score;
	}
}

void GoToFountain::enter(Agent * agent, float dt)
{
	m_currentDistance = 0;
	m_closestFountain = nullptr;


	if (agent->getFountains() != nullptr)
	{
		for (auto& fountain : *agent->getFountains())
		{
			if (m_currentDistance == 0)
			{
				m_closestFountain = &fountain;
				m_currentDistance = glm::length(fountain.position - agent->getPostion());
			}
			else if (glm::length(fountain.position - agent->getPostion()) < m_currentDistance)
			{
				m_closestFountain = &fountain;
				m_currentDistance = glm::length(fountain.position - agent->getPostion());
			}
		}
	}

	if (m_closestFountain)
	{
		generatePath(agent, m_closestFountain->position);
	}
	
}

void GoToFountain::exit(Agent * agent, float dt)
{
	agent->setCurrentAction(nullptr);
	m_currentPath.clear();
}

void GoToFountain::updateAction(Agent * agent, float dt)
{
	aie::Gizmos::addRing(m_closestFountain->position, m_closestFountain->effectRadius, m_closestFountain->effectRadius + 0.2f, 10, agent->getColour());
	if (glm::length(agent->getPostion() - m_closestFountain->position) > m_closestFountain->effectRadius - agent->getRadius())
	{
		followPath(agent, dt);
	}
	if (agent->getHealthPercentage() > .95f && agent->getManaPercentage() > .95f)
	{
		exit(agent, dt);
	}
}
