#include "Attack.h"
#include "Agent.h"

#include <algorithm>

Attack::Attack()
{
}

Attack::~Attack()
{
}

float Attack::evaluate(Agent * agent, float dt)
{
	return 0.0f;
}

void Attack::enter(Agent* agent, float dt)
{

}

void Attack::exit(Agent* agent, float dt)
{
}

void Attack::updateAction(Agent* agent, float dt)
{

}

void Attack::updateTimer(float dt)
{
	if (m_CDTimer > 0)
		m_CDTimer -= dt;

	if (m_CDTimer < 0)
		m_CDTimer = 0;
}

void Attack::applyDamage(Agent* agent)
{
	agent->subMana(m_cost);
	agent->getTarget()->takeDamage(m_damageMultiplier * agent->getAttackDamage());
	m_isCasting = false;
	m_castTimer = 0;
	m_CDTimer = m_coolDown;
}

bool Attack::needNewPath(Agent* agent)
{
	return (m_currentPath.size() == 0 || glm::length(m_currentPath.back()->position - agent->getTarget()->getPostion()) > 1.0f);
}

float Attack::checkCoolDown(Agent* agent)
{
	//return	std::min((agent->getDistanceToTarget() / (agent->getMoveSpeed()) / (m_CDTimer + 0.01f)), 1.0f);
	return (m_CDTimer <= 0);
}

float Attack::checkMana(Agent * agent)
{
	//return (agent->getCurrentMana >= m_cost) ? 1.0f : agent->getCurrentMana / m_cost;
	return (agent->getCurrentMana() >= m_cost) ? 1.0f : 0.f;
}

float Attack::checkDPS(Agent * agent)
{
	float damage = agent->getAttackDamage() * m_damageMultiplier;
	return (m_castTime >= 1) ? damage / m_castTime + damage : damage * m_castTime + damage;
}
