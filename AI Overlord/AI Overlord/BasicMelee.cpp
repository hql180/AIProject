#include "BasicMelee.h"
#include "Agent.h"
#include "Gizmos.h"

BasicMelee::BasicMelee() 
{
	m_cost = 0;
	m_attackRange = 0.5f;
	m_damageMultiplier = 1.0f;
	m_coolDown = 1;
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
	float score = 3;

	score += checkDPS(agent) * checkMana(agent) * checkCoolDown(agent);

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
	if (agent->getDistanceToTarget() > m_attackRange && !m_isCasting)
	{
		if (m_currentPath.size() == 0 || glm::length(m_currentPath.back()->position - agent->getTarget()->getPostion()) > 1.0f)
		{
			generatePath(agent, agent->getTarget()->getPostion());
		}

		followPath(agent, dt);
	}
	else if (agent->getDistanceToTarget() <= m_attackRange && m_CDTimer <= 0 || m_isCasting && m_CDTimer <= 0)
	{
		m_isCasting = true;
		m_castTimer += dt;

		if (m_castTimer >= m_castTime)
		{
			m_CDTimer = m_coolDown;
			m_castTimer = 0;
			agent->getTarget()->setPosition(glm::vec3(0));
			m_isCasting = false;
			exit(agent, dt);
		}
	}
}

void BasicMelee::updateTimer(float dt)
{
	Attack::updateTimer(dt);
}
