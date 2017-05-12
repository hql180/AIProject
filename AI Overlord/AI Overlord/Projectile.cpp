#include "Projectile.h"
#include "Agent.h"
#include "Attack.h"
#include "Gizmos.h"

Projectile::Projectile() : m_isActive(false)
{
}


Projectile::~Projectile()
{
}

void Projectile::update(float dt)
{
	if (m_isActive)
	{
		glm::vec3 force = glm::vec3(0);

		if (m_homingRate > 0)
		{
			force = glm::normalize(m_currentTarget->getPostion() - m_pos) * m_homingRate;
		}

		if (m_vel != glm::vec3(0))
			m_vel = glm::normalize(m_vel + force * dt) * m_speed;

		m_pos = m_pos + m_vel * dt;

		m_distanceTravelled = m_distanceTravelled + glm::length(m_vel * dt);

		aie::Gizmos::addSphere(m_pos, m_radius, 3, 3, m_colour);

		if (m_distanceTravelled >= m_range)
		{
			m_isActive = false;
			aie::Gizmos::addRing(m_pos, 0.2f, m_AOERadius, 10, m_shooter->getColour());

			if (m_AOERadius > 0)
			{
				for (auto& target : m_shooter->getAllAgents())
				{
					if (m_shooter != target && glm::length(target->getPostion() - m_pos) <= m_AOERadius)
					{						
						m_currentAttack->applyDamage(target, m_damage);
					}
				}
			}
		}
		else for (auto& target : m_shooter->getAllAgents())
		{
			if (m_shooter != target && glm::length(target->getPostion() - m_pos) <= m_radius + target->getRadius())
			{
				aie::Gizmos::addRing(m_pos, 0.1f, m_AOERadius, 10, m_shooter->getColour());
				m_isActive = false;
				if (m_AOERadius > 0)
				{
					for (auto& target : m_shooter->getAllAgents())
					{
						if (m_shooter != target && glm::length(target->getPostion() - m_pos) <= m_AOERadius)
						{
							m_currentAttack->applyDamage(target, m_damage);
						}
					}
					break;
				}				
				m_currentAttack->applyDamage(target, m_damage);				
			}
		}
	}
}



void Projectile::shoot(Attack * attack, Agent* agent, float homingRate, float aoe)
{
	m_currentAttack = attack;
	m_currentTarget = agent->getTarget();
	m_shooter = agent;
	m_speed = attack->m_projectileSpeed;
	m_range = attack->m_attackRange * 1.5f;
	m_radius = attack->m_radius;
	m_distanceTravelled = 0;
	m_damage = attack->m_damageMultiplier * agent->getAttackDamage();
	m_pos = agent->getPostion();
	m_vel = glm::normalize(m_currentTarget->getPostion() - agent->getPostion()) * m_speed;
	m_colour = agent->getColour();
	m_homingRate = homingRate;
	m_AOERadius = aoe;
	m_isActive = true;
}

bool Projectile::getActive()
{
	return m_isActive;
}
