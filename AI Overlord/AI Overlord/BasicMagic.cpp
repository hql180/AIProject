#include "BasicMagic.h"
#include "Agent.h"
#include "Gizmos.h"

BasicMagic::BasicMagic()
{
	m_cost = 20.f;
	m_attackRange = 7.f;
	m_damageMultiplier = 2.f;
	m_CD = 5.f;
	m_CDTimer = 0;
	m_castTime = 1.55f;
	m_castTimer = 0;
	m_radius = 0.3f;
	m_projectileSpeed = 5.f;

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

	score += checkDPS(agent) * checkMana(agent) * checkCoolDown(agent);

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
	agent->checkDistanceToTarget();
	if (agent->getDistanceToTarget() > m_attackRange && !m_isCasting)
	{
		seekTarget(agent, dt);
	}
	else if (agent->getDistanceToTarget() <= m_attackRange && m_CDTimer <= 0 || m_isCasting && m_CDTimer <= 0)
	{
		m_isCasting = true;
		m_castTimer += dt;

		agent->setVelocity(agent->getDirectionToTarget());

		if (m_castTimer >= m_castTime)
		{
			finishAttack(agent);
			for (auto& projectile : m_projectTilePool)
			{
				if (!projectile.getActive())
				{
					projectile.shoot(this, agent, 3.f, 7.5f);
					
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
