#include "Projectile.h"
#include "Agent.h"
#include "Attack.h"
#include "Gizmos.h"

Projectile::Projectile() : m_isActive(false), m_displayTime(0)
{
}


Projectile::~Projectile()
{
}

void Projectile::update(float dt)
{
	if (m_isActive)
	{
		if (m_pos.y > m_currentTarget->getPostion().y)
		{
			m_pos.y -= dt;
		}

		glm::vec3 force = glm::vec3(0);

		if (m_homingRate > 0)
		{
			force = glm::normalize(m_currentTarget->getPostion() - m_pos) * m_homingRate;
		}

		if (m_vel != glm::vec3(0))
			m_vel = glm::normalize(m_vel + force * dt) * m_speed;

		m_pos = m_pos + m_vel * dt;

		m_distanceTravelled = m_distanceTravelled + glm::length(m_vel * dt);

		aie::Gizmos::addSphere(m_pos, m_radius, 4, 4, m_colour);

		for (auto& obs : *m_shooter->getObstacle())
		{
			if (glm::length(obs.position - m_pos) <= obs.radius + m_radius)
			{
				m_isActive = false;
				if (m_AOERadius > 0)
				{
					for (auto& target : m_shooter->getAllAgents())
					{
						if (m_shooter != target && glm::length(target->getPostion() - m_pos) <= m_AOERadius + target->getRadius())
						{
							m_currentAttack->applyDamage(target, m_damage);
						}
					}
				}
				break;
			}
		}


		if (m_distanceTravelled >= m_range)
		{
			m_isActive = false;
			if (m_AOERadius > 0)
			{
				for (auto& target : m_shooter->getAllAgents())
				{
					if (m_shooter != target && glm::length(target->getPostion() - m_pos) <= m_AOERadius + target->getRadius())
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
				m_isActive = false;
				if (m_AOERadius > 0)
				{
					aie::Gizmos::addRing(m_pos, 0.1f, m_AOERadius, 10, m_shooter->getColour());
					for (auto& target : m_shooter->getAllAgents())
					{
						if (m_shooter != target && glm::length(target->getPostion() - m_pos) <= m_AOERadius + target->getRadius())
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

	if (!m_isActive && m_displayTime > 0)
	{
		aie::Gizmos::addRing(m_pos, 0, m_AOERadius, 10, m_shooter->getColour());
		m_displayTime -= dt;
	}
}



void Projectile::shoot(Attack * attack, Agent* agent, float homingRate, float aoe, glm::vec3 pos, float range, float radius)
{
	m_currentAttack = attack;
	m_currentTarget = agent->getTarget();
	m_shooter = agent;
	m_speed = attack->m_projectileSpeed;
	m_range = attack->m_attackRange * range;
	
	m_distanceTravelled = 0;
	m_damage = attack->m_damageMultiplier * agent->getAttackDamage();

	if (pos == glm::vec3(0))
		m_pos = agent->getPostion();
	else
		m_pos = pos;

	m_vel = glm::normalize(m_currentTarget->getPostion() - agent->getPostion()) * m_speed;
	m_colour = agent->getColour();
	m_homingRate = homingRate;
	m_AOERadius = aoe;

	if (radius == 0)
		m_radius = (m_AOERadius < attack->m_radius && m_AOERadius != 0) ? m_AOERadius : attack->m_radius;
	else
		m_radius = radius;

	m_isActive = true;
	m_displayTime = 0.1f;
}

bool Projectile::getActive()
{
	return m_isActive;
}
