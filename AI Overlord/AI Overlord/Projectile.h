#pragma once
#include "glm.hpp"

class Agent;
class Attack;
class Projectile
{
public:
	Projectile();
	
	~Projectile();

	void update(float dt);

	void shoot(Attack* attack, Agent* agent, float homingRate = 0, float aoe = 0, glm::vec3 pos = glm::vec3(0), float range = 2.f, float radius = 0);

	bool getActive();

private:
	float m_speed;
	float m_range;
	float m_radius, m_AOERadius;
	float m_homingRate;
	float m_distanceTravelled;
	float m_damage;
	float m_displayTime;
	bool m_isActive;
	
	Attack* m_currentAttack;
	Agent* m_currentTarget;
	Agent* m_shooter;

	glm::vec3 m_pos;
	glm::vec3 m_vel;
	glm::vec4 m_colour;
};

