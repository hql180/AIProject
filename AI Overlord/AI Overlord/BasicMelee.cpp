#include "BasicMelee.h"
#include "Agent.h"

BasicMelee::BasicMelee() 
{
	m_cost = 0;
	m_attackRange = 0.5f;
	m_damageMultiplier = 1.0f;
	m_coolDown = 1;
	m_CDTimer = 0;
	m_castTime = 0.1f;
	m_castTimer = 0;
}

BasicMelee::~BasicMelee()
{
}

float BasicMelee::evaluate(Agent* agent, float dt)
{
	float score = 0;

	score += checkDPS(agent) * checkMana(agent) * checkCoolDown(agent);

	return score;
}

void BasicMelee::enter(Agent* agent, float dt)
{
	agent->setCurrentAction(this);
}

void BasicMelee::exit(Agent* agent, float dt)
{
	agent->setCurrentAction(agent->getEngage());
}

void BasicMelee::updateAction(Agent* agent, float dt)
{
	if (agent->getDistanceToTarget() <= m_attackRange && m_CDTimer <= 0)
	{
		m_castTimer += dt;

		if (m_castTimer >= m_castTime)
		{
			exit(agent, dt);
		}
	}
}
