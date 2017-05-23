#include "StrongMelee.h"
#include "Agent.h"
#include "Gizmos.h"


StrongMelee::StrongMelee()
{
	m_cost = 20.f;
	m_attackRange = 1.2f;
	m_damageMultiplier = 5.5f;
	m_CD = 5.5f;
	m_CDTimer = 0;
	m_castTime = 0.5f;
	m_castTimer = 0;
	m_isCasting = false;
}


StrongMelee::~StrongMelee()
{
}

float StrongMelee::evaluate(Agent * agent, float dt)
{
	float score = 0;

	score += checkDamage(agent) * agent->getHealthPercentage()* checkMana(agent) * checkCoolDown(agent);

	if (glm::length(agent->getPostion() - agent->getTarget()->getPostion()) > m_attackRange)
		score /= 3.f;

	return score;
}

void StrongMelee::enter(Agent * agent, float dt)
{
}

void StrongMelee::exit(Agent * agent, float dt)
{
	m_isCasting = false;
	m_castTimer = 0;
	agent->setCurrentAction(nullptr);
	m_currentPath.clear();
}

void StrongMelee::updateAction(Agent * agent, float dt)
{
	float distance = glm::length(agent->getPostion() - agent->getTarget()->getPostion());
	if (distance > m_attackRange && !m_isCasting)
	{
		seekTarget(agent, dt);
	}
	else if (distance <= m_attackRange && m_CDTimer <= 0 || m_isCasting && m_CDTimer <= 0)
	{
		m_isCasting = true;
		m_castTimer += (dt * (1.f + agent->getStats().agility * 0.01f));



		if (m_castTimer >= m_castTime)
		{

			finishAttack(agent);
			applyDamage(agent->getTarget(), agent->getAttackDamage() * m_damageMultiplier);
			exit(agent, dt);
		}
	}
}

void StrongMelee::updateTimer(float dt)
{
	Attack::updateTimer(dt);
}
