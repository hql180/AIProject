#include "Charge.h"
#include "Agent.h"


Charge::Charge()
{
	name = "Charge";
	m_cost = 5.f;
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
	if (glm::length(agent->getPostion() - agent->getETarget()->getPostion()) < 2.f)
		return 0;

	if (checkMana(agent) == 0 || checkCoolDown(agent) == 0)
	{
		return 0;
	}
	float score = checkDamage(agent) / agent->getETarget()->getCurrentHealth();

	if (score > 1)
		score = 1.f;

	score += agent->getHealthPercentage();

	score = score + (agent->getHealthPercentage() / agent->getETarget()->getHealthPercentage() < 1) ? agent->getHealthPercentage() / agent->getETarget()->getHealthPercentage() : 1.f;

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
		m_CDTimer *= 1.f - (agent->getStats().intelligence * 0.01);
	}
	m_castTimer = 0;
	agent->setCurrentAction(nullptr);
	m_currentPath.clear();
}

void Charge::updateAction(Agent * agent, float dt)
{
	float distance = glm::length(agent->getPostion() - agent->getTarget()->getPostion());
	if (!m_isCasting || distance > m_attackRange)
	{
		seekTarget(agent, dt);
	}
	if (distance <= m_attackRange)
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
			applyDamage(agent->getTarget(), agent->getAttackDamage() * 0.5f);
			exit(agent, dt);
		}
	}


}

void Charge::updateTimer(float dt)
{
	Attack::updateTimer(dt);
}
