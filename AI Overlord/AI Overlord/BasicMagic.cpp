#include "BasicMagic.h"
#include "Agent.h"
#include "Gizmos.h"

BasicMagic::BasicMagic()
{
	m_cost = 5.f;
	m_attackRange = 8.f;
	m_damageMultiplier = 2.5f;
	m_CD = 1.f;
	m_CDTimer = 0;
	m_castTime = 2.f;
	m_castTimer = 0;
	m_radius = 0.8f;
	m_projectileSpeed = 6.f;

	m_isCasting = false;
	m_projectTilePool.push_back(Projectile());
	m_projectTilePool.push_back(Projectile());
}


BasicMagic::~BasicMagic()
{
}

float BasicMagic::evaluate(Agent * agent, float dt)
{
	float score = 0;

	score += checkDamage(agent) * checkMana(agent) * checkCoolDown(agent);

	return score;
}

void BasicMagic::enter(Agent * agent, float dt)
{

}

void BasicMagic::exit(Agent * agent, float dt)
{
	m_isCasting = false;

	m_castTimer = 0;
	agent->setCurrentAction(nullptr);
	m_currentPath.clear();
}

void BasicMagic::updateAction(Agent * agent, float dt)
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

		agent->setVelocity(agent->getDirectionToTarget());

		if (m_castTimer >= m_castTime)
		{
			finishAttack(agent);
			for (auto& projectile : m_projectTilePool)
			{
				if (!projectile.getActive())
				{
					projectile.shoot(this, agent, 5.f, 0.5f);
					
					break;
				}
			}
			exit(agent, dt);
		}
	}
}

void BasicMagic::updateTimer(float dt)
{
	Attack::updateTimer(dt);
	for (auto& projectile : m_projectTilePool)
	{
		projectile.update(dt);
	}
}
