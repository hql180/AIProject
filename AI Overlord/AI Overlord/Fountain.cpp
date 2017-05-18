#include "Fountain.h"
#include "Agent.h"
#include "Gizmos.h"



Fountain::Fountain()
{
}

Fountain::Fountain(glm::vec3 pos, glm::vec3 ext, float rad, float effectRad, float heal, float mana)
{
	position = pos;
	extent = ext;
	radius = rad;
	effectRadius = effectRad;
	healthRate = heal;
	manaRate = mana;
}


Fountain::~Fountain()
{
}

void Fountain::update(std::vector<Agent*> agents, float dt)
{
	for (auto& agent : agents)
	{
		float distance = glm::length(agent->getPostion() - position);
		if (distance <= effectRadius)
		{
			agent->addHealth(healthRate * dt);
			agent->addMana(manaRate * dt);
			aie::Gizmos::addDisk(agent->getPostion(), agent->getRadius(), 5, glm::vec4(.1, .2, .3, 1));
		}
	}

	aie::Gizmos::addAABBFilled(glm::vec3(position.x, position.y + extent.y, position.z) , extent, glm::vec4(.53f, .231f, .828f, 1.f));
	aie::Gizmos::addSphere(glm::vec3(position.x, position.y + radius, position.z), radius, 5, 5, glm::vec4(.369f, .635f, .976f, 1.f));
	aie::Gizmos::addDisk(position, effectRadius, 15, glm::vec4(.231f, .937f, .549f, .5f));

}
