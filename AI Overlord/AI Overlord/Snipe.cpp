#include "Snipe.h"
#include "Agent.h"
#include "Projectile.h"
#include "Gizmos.h"


Snipe::Snipe()
{
	name = " ";
	m_cost = 20;
	m_attackRange = 14.f;
	m_damageMultiplier = 4.5f;
	m_CD = 8.f;
	m_CDTimer = 0;
	m_castTime = 3.5f;
	m_castTimer = 0;
	m_radius = 0.2f;
	m_projectileSpeed = 20.f;
	m_isCasting = false;

	m_projectTilePool.push_back(Projectile());
	m_projectTilePool.push_back(Projectile());
}


Snipe::~Snipe()
{
}

float Snipe::evaluate(Agent * agent, float dt)
{
	float distance = glm::length(agent->getPostion() - agent->getETarget()->getPostion());

	if (distance > m_attackRange)
		distance = m_attackRange;

	m_cost = 15 + distance * 0.33f;

	m_damageMultiplier = 4.5f * distance / m_attackRange;
	
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

void Snipe::enter(Agent * agent, float dt)
{
}

void Snipe::exit(Agent * agent, float dt)
{
	m_isCasting = false;
	m_castTimer = 0;
	agent->setCurrentAction(nullptr);
	m_currentPath.clear();
}

void Snipe::updateAction(Agent * agent, float dt)
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
					projectile.shoot(this, agent, 1.f);
					break;
				}
			}
			exit(agent, dt);
		}
	}
}

void Snipe::updateTimer(float dt)
{
	Attack::updateTimer(dt);
	for (auto& projectile : m_projectTilePool)
		projectile.update(dt);
}
