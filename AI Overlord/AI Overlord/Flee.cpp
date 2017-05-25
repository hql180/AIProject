#include "Flee.h"
#include "Agent.h"
#include "Gizmos.h"


Flee::Flee() : m_corneredCounter(30.f)
{
}


Flee::~Flee()
{
}

float Flee::evaluate(Agent * agent, float dt)
{
	float score =  1.01f - agent->getHealthPercentage();

	if (agent->getHostiles().size() > 0)
	{
		float hScore = 0;
		for (auto& hostile : agent->getHostiles())
		{
			float healthDifference = hostile->getHealthPercentage() - agent->getHealthPercentage();
			if (healthDifference > 0)
			{
				hScore += healthDifference;
			}

			if (glm::length(hostile->getPostion() - agent->getPostion()) < agent->getPreferedRange())
			{
				hScore += 1.f * hostile->getHealthPercentage() * (glm::length(hostile->getPostion() - agent->getPostion()) / agent->getPreferedRange());
			}
		}
	

		score = 2.f * score + hScore ;
	}
	else
	{
		return 0;
	}

	return (score * (m_corneredCounter / 30.f));
}

void Flee::enter(Agent * agent, float dt)
{
	glm::vec3 destination = agent->getPostion();

	for (auto& hostile : agent->getHostiles())
	{
		destination = destination + (agent->getDirectionToTarget(hostile, agent) * (agent->getVisionRange() / 2.f));
	}

	if (glm::length(destination - agent->getPostion()) < 3.f)
		destination = glm::normalize(destination) * (agent->getVisionRange() / 2.f);

	generatePath(agent, destination);
}

void Flee::exit(Agent * agent, float dt)
{
	if (m_corneredCounter > 0)
	{
		m_corneredCounter -= 10.f;
	}

	printf("%f", m_corneredCounter);

	agent->setCurrentAction(nullptr);
}

void Flee::updateAction(Agent * agent, float dt)
{
	if (m_currentPath.size() > 0)
	{
		aie::Gizmos::addRing(m_currentPath.back()->position, 0.2f, 0.3f, 5, agent->getColour());
		followPath(agent, dt * (1.f + agent->getStats().agility * 0.02f));
	}
	else
	{
		exit(agent, dt);
	}
}

void Flee::updateTimer(float dt)
{
	if (m_corneredCounter < 9)
	{
		m_corneredCounter += dt;
		if (m_corneredCounter > 9)
			m_corneredCounter = 9;
	}
}




