#include "Attack.h"
#include "Agent.h"


Attack::Attack(Agent * agent) : m_targetAgent(agent)
{
}

Attack::~Attack()
{
}

float Attack::evaluate(Agent * agent)
{
	float score = 0;

	score += ((agent->getHealthPercentage() / (m_targetAgent->getHealthPercentage() + 0.01f))
		* (agent->getAttackDamage() / (m_targetAgent->getCurrentHealth() + 0.01f))
		+ (agent->getManaPercentage() / 3)); // arbitary 3 placeholder value

	return score;
}

void Attack::enter(Agent * agent)
{
	agent->setAttackTarget(m_targetAgent);
}

void Attack::exit(Agent * agent)
{
}

void Attack::updateAction(Agent * agent)
{
	float bestScore = 0;
	Action* bestAction = nullptr;
	
	for (auto& action : agent->getAttackList())
	{
		float score = action.evaluate(agent);
		if (score > bestScore)
		{
			bestScore = score;
			bestAction = &action;
		}
	}

	bestAction->enter(agent);
}
