#include "Engage.h"
#include "Agent.h"


Engage::Engage()
{
}

Engage::Engage(Agent * agent) : m_targetAgent(agent)
{
}


Engage::~Engage()
{
}

float Engage::evaluate(Agent* agent, float dt)
{
	float score = 0;

	//score += ((agent->getHealthPercentage() / (m_targetAgent->getHealthPercentage() + 0.01f))
	//	* (agent->getAttackDamage() / (m_targetAgent->getCurrentHealth() + 0.01f))
	//	+ (agent->getMoveSpeed() / ((glm::length(agent->getPostion() - m_targetAgent->getPostion())) + 0.01f))
	//	+ (agent->getManaPercentage() / 3)); // arbitary 3 placeholder value

	return 2;
}

void Engage::enter(Agent* agent, float dt)
{
	agent->setAttackTarget(m_targetAgent);	
	agent->setCurrentAction(this);
}

void Engage::exit(Agent* agent, float dt)
{
	agent->setAttackTarget(nullptr);
	agent->setCurrentAction(nullptr);
}

void Engage::updateAction(Agent* agent, float dt)
{
	if (false && m_targetAgent->getCurrentHealth() <= 0)
	{
		exit(agent, dt);
	}
	else
	{
		float bestScore = 0;
		Action* bestAction = nullptr;
		agent->checkDistanceToTarget();

		for (auto& action : agent->getTActions())
		{
			float score = action->evaluate(agent, dt);
			if (score > bestScore)
			{
				bestScore = score;
				bestAction = action;
			}
		}
		
		bestAction->enter(agent, dt);
	}
}

void Engage::updateTimer(float dt)
{
}
