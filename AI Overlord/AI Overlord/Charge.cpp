#include "Charge.h"
#include "Agent.h"


Charge::Charge()
{
	m_cost = 10.f;
	m_attackRange = 7.f;
	m_damageMultiplier = 3.0f;
	m_CD = 7.f;
	m_CDTimer = 0;
	m_castTime = 0.f;
	m_castTimer = 0;
	m_isCasting = false;
}


Charge::~Charge()
{
}

float Charge::evaluate(Agent * agent, float dt)
{
	float score = 0;

	score += checkDamage(agent) * agent->getHealthPercentage() * checkMana(agent) * checkCoolDown(agent);

	return score;
}

void Charge::enter(Agent * agent, float dt)
{

}

void Charge::exit(Agent * agent, float dt)
{
	if (m_isCasting)
	{
		m_isCasting = false;
		m_CD *= 1.f - (agent->getStats().intelligence * 0.01);
	}
	m_castTimer = 0;
	agent->setCurrentAction(nullptr);
	m_currentPath.clear();
}

void Charge::updateAction(Agent * agent, float dt)
{
	float distance = glm::length(agent->getPostion() - agent->getTarget()->getPostion());
	if (distance > m_attackRange && !m_isCasting)
	{
		seekTarget(agent, dt);
	}
	else if (distance <= m_attackRange)
	{
		m_isCasting = true;
		m_CDTimer = m_CD;
		if (m_currentPath.size() == 0)
		{
			generatePath(agent, agent->getTarget()->getPostion());
		}

		followPath(agent, dt * m_damageMultiplier);

		if (m_currentPath.size() == 0 || glm::length(agent->getPostion() - agent->getTarget()->getPostion()) < agent->getRadius() + agent->getTarget()->getRadius())
		{
			finishAttack(agent);
			exit(agent, dt);
		}
	}
}

void Charge::updateTimer(float dt)
{
	Attack::updateTimer(dt);
}
