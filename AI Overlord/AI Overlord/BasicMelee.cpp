#include "BasicMelee.h"
#include "Agent.h"
#include "Gizmos.h"
#include <iostream>

BasicMelee::BasicMelee() 
{
	m_cost = 0;
	m_attackRange = 1.f;
	m_damageMultiplier = 2.0f;
	m_coolDown = 1.f;
	m_CDTimer = 0;
	m_castTime = 0.1f;
	m_castTimer = 0;
	m_isCasting = false;
}

BasicMelee::~BasicMelee()
{
}

float BasicMelee::evaluate(Agent* agent, float dt)
{
	float score = 0;

	score += checkDPS(agent) * checkMana(agent) * checkCoolDown(agent);

	std::cout << score << std::endl;

	return score;
}

void BasicMelee::enter(Agent* agent, float dt)
{
	agent->setCurrentAction(this);
}

void BasicMelee::exit(Agent* agent, float dt)
{
	//agent->setCurrentAction(agent->getTarget()->getEngage());
	m_isCasting = false;
	agent->setCurrentAction(nullptr);
	m_currentPath.clear();
}

void BasicMelee::updateAction(Agent* agent, float dt)
{
	agent->checkDistanceToTarget();
	if (agent->getDistanceToTarget() > m_attackRange && !m_isCasting)
	{
		if (needNewPath(agent))
		{
			generatePath(agent, agent->getTarget()->getPostion());
		}

		followPath(agent, dt);
		aie::Gizmos::addRing(agent->getTarget()->getPostion(), 0.2f, 0.3f, 5, agent->getColour());
	}
	else if (agent->getDistanceToTarget() <= m_attackRange && m_CDTimer <= 0 || m_isCasting && m_CDTimer <= 0)
	{
		m_isCasting = true;
		m_castTimer += dt;

		if (m_castTimer >= m_castTime)
		{			
			applyDamage(agent);			
			exit(agent, dt);
		}
	}
}

void BasicMelee::updateTimer(float dt)
{
	Attack::updateTimer(dt);
}
