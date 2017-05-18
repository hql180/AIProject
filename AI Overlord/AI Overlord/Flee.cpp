#include "Flee.h"
#include "Agent.h"
#include "Gizmos.h"


Flee::Flee() : m_corneredCounter(5)
{
}


Flee::~Flee()
{
}

float Flee::evaluate(Agent * agent, float dt)
{
	float score =  1.1f - agent->getHealthPercentage();

	if (agent->getHostiles().size() > 0)
	{
		float hScore = 0;
		for (auto& hostile : agent->getHostiles())
		{
			hScore += glm::max((hostile->getHealthPercentage() - agent->getHealthPercentage()), 0.f);
			hScore = hScore + glm::max(glm::length(hostile->getPostion() - agent->getPostion()) - agent->getPreferedRange(), 0.f) * hostile->getHealthPercentage();
		}
		
		//hScore /= agent->getHostiles().size();

		score = 2.f * score + hScore;
	}
	else
	{
		return 0;
	}

	printf("flee score: %f \n", score * (m_corneredCounter / 5.f)); //Debug

	return score * (m_corneredCounter / 5.f);
}

void Flee::enter(Agent * agent, float dt)
{
	glm::vec3 destination = agent->getPostion();

	for (auto& hostile : agent->getHostiles())
	{
		//destination = destination + (agent->getDirectionToTarget(hostile, agent) * (agent->getVisionRange() - glm::length(agent->getPostion() - hostile->getPostion())));
		destination = destination + (agent->getDirectionToTarget(hostile, agent) * agent->getMoveSpeed());
	}

	generatePath(agent, destination);
}

void Flee::exit(Agent * agent, float dt)
{
	if (m_corneredCounter > 0)
	{
		m_corneredCounter--;
	}

	agent->setCurrentAction(nullptr);
}

void Flee::updateAction(Agent * agent, float dt)
{
	if (m_currentPath.size() > 0)
	{
		aie::Gizmos::addRing(m_currentPath.back()->position, 0.2f, 0.3f, 5, agent->getColour());
		followPath(agent, dt);		
	}
	else
	{
		exit(agent, dt);
	}
}

void Flee::updateTimer(float dt)
{
	if (m_corneredCounter < 5)
	{
		m_corneredCounter += dt;
		if (m_corneredCounter > 5)
			m_corneredCounter = 5;
	}
}




