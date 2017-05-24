#pragma once
#include "glm.hpp"

#include <vector>

class Agent;
class Fountain
{
public:
	Fountain();
	Fountain(glm::vec3 pos, glm::vec3 ext = glm::vec3(.5, .25, .5), float rad = .5f, float effectRad = 2.5f, float heal = 5.f, float mana = 2.5f);
	~Fountain();

	void update(std::vector<Agent*> agents, float dt);

	glm::vec3 position;
	glm::vec3 extent;
	float radius;
	float effectRadius;
	float healthRate;
	float manaRate;
};

