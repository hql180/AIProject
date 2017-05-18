#include "BasicMelee.h"
#include "Agent.h"
#include "Gizmos.h"
#include <iostream>

BasicMelee::BasicMelee() 
{
	m_cost = 0;
	m_attackRange = 1.f;
	m_damageMultiplier = 2.0f;
	m_CD = 1.f;
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

	return score;
}

void BasicMelee::enter(Agent* agent, float dt)
{
	
}

void BasicMelee::exit(Agent* agent, float dt)
{
	//agent->setCurrentAction(agent->getTarget()->getEngage());
	m_isCasting = false;
	m_castTimer = 0;
	agent->setCurrentAction(nullptr);
	m_currentPath.clear();
}

void BasicMelee::updateAction(Agent* agent, float dt)
{
	agent->checkDistanceToTarget();
	if (agent->getDistanceToTarget() > m_attackRange && !m_isCasting)
	{
		seekTarget(agent, dt);
	}
	else if (agent->getDistanceToTarget() <= m_attackRange && m_CDTimer <= 0 || m_isCasting && m_CDTimer <= 0)
	{
		m_isCasting = true;
		m_castTimer += dt;

		if (m_castTimer >= m_castTime)
		{	
			finishAttack(agent);
			applyDamage(agent->getTarget(), agent->getAttackDamage() * m_damageMultiplier);			
			exit(agent, dt);
		}
	}
}

void BasicMelee::updateTimer(float dt)
{
	Attack::updateTimer(dt);
}
