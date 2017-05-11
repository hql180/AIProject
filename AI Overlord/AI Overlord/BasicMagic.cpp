#include "BasicMagic.h"
#include "Agent.h"
#include "Gizmos.h"

BasicMagic::BasicMagic()
{
	m_cost = 0;
	m_attackRange = 4.f;
	m_damageMultiplier = 3.5f;
	m_coolDown = 2.f;
	m_CDTimer = 0;
	m_castTime = 1.f;
	m_castTimer = 0;
	m_isCasting = false;
}


BasicMagic::~BasicMagic()
{
}

float BasicMagic::evaluate(Agent * agent, float dt)
{
	float score = 0;

	score += checkDPS(agent) * checkMana(agent) * checkCoolDown(agent);

	printf("DPS: %f MANA: %f CoolDown: %f \n", checkDPS(agent), checkMana(agent), checkCoolDown(agent));

	return score;
}

void BasicMagic::enter(Agent * agent, float dt)
{
}

void BasicMagic::exit(Agent * agent, float dt)
{
}

void BasicMagic::updateAction(Agent * agent, float dt)
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

void BasicMagic::updateTimer(float dt)
{
	Attack::updateTimer(dt);
}
