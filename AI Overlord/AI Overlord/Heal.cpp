#include "Heal.h"
#include "Agent.h"
#include "Gizmos.h"

Heal::Heal()
{
	m_cost = 20.f;
	m_attackRange = 0.f;
	m_damageMultiplier = 1.f;
	m_CD = 8.f;
	m_CDTimer = 0;
	m_castTime = 3.f;
	m_castTimer = 0;
	m_radius = 0.3f;

	m_isCasting = false;
}


Heal::~Heal()
{
}

float Heal::evaluate(Agent * agent, float dt)
{
	if (m_CDTimer > 0)
	{
		return 0.f;
	}
	float score = 1.1f - agent->getHealthPercentage();

	return score;
}

void Heal::enter(Agent * agent, float dt)
{
}

void Heal::exit(Agent * agent, float dt)
{
	m_isCasting = false;
	m_castTimer = 0;
	agent->setCurrentAction(nullptr);
	m_currentPath.clear();
}

void Heal::updateAction(Agent * agent, float dt)
{
	if (!m_isCasting)
	{
		m_isCasting = true;
		m_CDTimer = m_CD;
	}
	else 
	{
		m_castTimer += dt;

		agent->addHealth(agent->getStats().vitality * m_damageMultiplier * dt);

		aie::Gizmos::addRing(agent->getPostion(), agent->getRadius() * 0.7f, agent->getRadius() * 1.3f, 10, glm::vec4(.2, .7, .4, 1));

		if (m_castTimer > m_castTime || agent->getHealthPercentage() > 1)
		{
			exit(agent, dt);
		}
	}
}

void Heal::updateTimer(float dt)
{
	Attack::updateTimer(dt);
}
