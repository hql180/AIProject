#pragma once
#include "glm.hpp"

class Agent;
class Projectile
{
public:
	Projectile();
	
	~Projectile();

	void update(Agent* agent, float dt);

private:
	float m_speed;
	float m_distance;
	float m_distanceTravelled;
	float m_damage;

	glm::vec3 m_pos;
	glm::vec3 m_vel;
	glm::vec3 m_colour;
};

