#include "Projectile.h"
#include "Agent.h"


Projectile::Projectile()
{
}


Projectile::~Projectile()
{
}

void Projectile::update(Agent* agent, float dt)
{
	m_pos = m_pos + m_vel * dt;

	m_distanceTravelled = m_distanceTravelled + glm::length(m_vel * dt);


}
