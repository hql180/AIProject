#include "Wander.h"
#include "Agent.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(-5.0f, 5.0f);

Wander::Wander()
{
}


Wander::~Wander()
{
}

float Wander::evaluate(Agent * agent, float dt)
{
	return 1.0f;
}

void Wander::enter(Agent * agent, float dt)
{
	glm::vec3 target = agent->getPostion() + (((glm::length(agent->getVelocity()) != 0) ? glm::normalize(agent->getVelocity()) : agent->getVelocity()) * 2.5f);
	glm::vec3 displacement = glm::normalize(glm::vec3(dis(gen), 0, dis(gen)));
		
	generatePath(agent, target + displacement);
}

void Wander::exit(Agent * agent, float dt)
{
	agent->setCurrentAction(nullptr);
}

void Wander::updateAction(Agent * agent, float dt)
{
	if (m_currentPath.size() > 0)
	{
		followPath(agent, dt);
	}
	else
	{
		exit(agent, dt);
	}
}

