#include "StrongMelee.h"
#include "Agent.h"
#include "Gizmos.h"


StrongMelee::StrongMelee()
{
	
	name = "Strong Melee";
	m_cost = 20.f;
	m_attackRange = 1.f;
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
	if (checkMana(agent) == 0 || checkCoolDown(agent) == 0)
	{
		return 0;
	}
	float score = checkDamage(agent) / agent->getETarget()->getCurrentHealth();

	if (score > 1)
		score = 1.f;

	score += agent->getHealthPercentage();

	score = score + (agent->getHealthPercentage() / agent->getETarget()->getHealthPercentage() < 1) ? agent->getHealthPercentage() / agent->getETarget()->getHealthPercentage() : 1.f;
	
	score = score - (glm::length(agent->getPostion() - agent->getETarget()->getPostion()) / agent->getVisionRange()) * 0.5f;

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
