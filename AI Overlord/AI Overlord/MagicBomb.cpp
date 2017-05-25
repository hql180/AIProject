#include "MagicBomb.h"
#include "Projectile.h"
#include "Agent.h"
#include "Gizmos.h"


MagicBomb::MagicBomb()
{
	m_cost = 25.f;
	m_attackRange = 10.f;
	m_baseDamage = 10.f;

	m_CD = 15.f;
	m_CDTimer = m_CD;
	m_castTime = 10.f;
	m_castTimer = 0;
	m_radius = 1.5f;
	m_projectileSpeed = 3.5f;

	m_isCasting = false;
	m_projectTilePool.push_back(Projectile());
	m_projectTilePool.push_back(Projectile());
}


MagicBomb::~MagicBomb()
{
}

float MagicBomb::evaluate(Agent * agent, float dt)
{
	m_damageMultiplier = m_baseDamage;

	if (checkMana(agent) == 0 || checkCoolDown(agent) == 0)
	{
		return 0;
	}
	float score = checkDamage(agent) / agent->getTarget()->getCurrentHealth();

	if (score > 1)
		score = 1.f;

	score += agent->getHealthPercentage();

	score = score + (agent->getHealthPercentage() / agent->getTarget()->getHealthPercentage() < 1) ? agent->getHealthPercentage() / agent->getTarget()->getHealthPercentage() : 1.f;

	return score;
}

void MagicBomb::enter(Agent * agent, float dt)
{
	m_bombReleased = false;
}

void MagicBomb::exit(Agent * agent, float dt)
{
	if (!m_bombReleased)
	{
		finishAttack(agent);
		for (auto& projectile : m_projectTilePool)
		{
			if (!projectile.getActive())
			{
				projectile.shoot(this, agent, 10.f, 5.f, agent->getPostion() + glm::vec3(0, m_radius + 5.f, 0), 5.f, m_radius * m_castTimer / m_castTime);

				break;
			}
		}
	}

	m_isCasting = false;

	m_castTimer = 0;
	agent->setCurrentAction(nullptr);
	
	m_currentPath.clear();
}

void MagicBomb::updateAction(Agent * agent, float dt)
{
	aie::Gizmos::addRing(agent->getTarget()->getPostion(), 0.2f, 0.3f, 5, agent->getColour());
	float distance = glm::length(agent->getPostion() - agent->getTarget()->getPostion());
	if (distance > m_attackRange && !m_isCasting)
	{
		seekTarget(agent, dt);
	}
	else if (distance <= m_attackRange && m_CDTimer <= 0 || m_isCasting && m_CDTimer <= 0)
	{
		m_isCasting = true;
		m_castTimer += (dt * (1.f + agent->getStats().agility * 0.01f));
		
		aie::Gizmos::addSphere(agent->getPostion() + glm::vec3(0,  m_radius + 5.f, 0), m_radius * m_castTimer / m_castTime, 4, 4, agent->getColour());

		m_damageMultiplier = m_baseDamage * m_castTimer / m_castTime;

		agent->setVelocity(agent->getDirectionToTarget());

		if (m_castTimer >= m_castTime)
		{
			finishAttack(agent);
			m_bombReleased = true;
			for (auto& projectile : m_projectTilePool)
			{
				if (!projectile.getActive())
				{
					projectile.shoot(this, agent, 10.f, 5.f, agent->getPostion() + glm::vec3(0, m_radius + 5.f, 0), 5.f, m_radius * m_castTimer / m_castTime);

					break;
				}
			}
			exit(agent, dt);
		}
	}
}

void MagicBomb::updateTimer(float dt)
{
	Attack::updateTimer(dt);
	for (auto& projectile : m_projectTilePool)
	{
		projectile.update(dt);
	}
}
