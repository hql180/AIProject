#include "basicRange.h"
#include "Projectile.h"
#include "Agent.h"
#include "Gizmos.h"

BasicRange::BasicRange()
{
	m_cost = 0;
	m_attackRange = 8.f;
	m_damageMultiplier = 1.f;
	m_CD = 1.f;
	m_CDTimer = 0;
	m_castTime = 0.5f;
	m_castTimer = 0;
	m_radius = 0.2f;
	m_projectileSpeed = 8.f;
	m_isCasting = false;

	m_projectTilePool.push_back(Projectile());
	m_projectTilePool.push_back(Projectile());
	m_projectTilePool.push_back(Projectile());
}


BasicRange::~BasicRange()
{
}

float BasicRange::evaluate(Agent * agent, float dt)
{
	float score = 0;

	score += checkDamage(agent) * checkMana(agent) * checkCoolDown(agent);

	return score;
}

void BasicRange::enter(Agent * agent, float dt)
{
	
}

void BasicRange::exit(Agent * agent, float dt)
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

void BasicRange::updateAction(Agent * agent, float dt)
{
	float distance = glm::length(agent->getPostion() - agent->getTarget()->getPostion());
	if (distance > m_attackRange && !m_isCasting)
	{
		if (needNewPath(agent))
		{
			generatePath(agent, agent->getTarget()->getPostion());
		}

		followPath(agent, dt);
		aie::Gizmos::addRing(agent->getTarget()->getPostion(), 0.2f, 0.3f, 5, agent->getColour());
	}
	else if (distance <= m_attackRange && m_CDTimer <= 0 || m_isCasting && m_CDTimer <= 0)
	{
		m_isCasting = true;
		m_castTimer += (dt * (1.f + agent->getStats().agility * 0.01f));

		agent->setVelocity(agent->getDirectionToTarget());

		if (m_castTimer >= m_castTime)
		{
			finishAttack(agent);
			for (auto& projectile : m_projectTilePool)
			{
				if (!projectile.getActive())
				{
					projectile.shoot(this, agent, 1.f);
					break;
				}
			}
			exit(agent, dt);
		}
	}
}

void BasicRange::updateTimer(float dt)
{
	Attack::updateTimer(dt);
	for (auto& projectile : m_projectTilePool)
		projectile.update(dt);
}
