#include "Wander.h"
#include "Agent.h"
#include "Gizmos.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(-5.0f, 5.0f);

Wander::Wander() : m_helper(0)
{
	name = " ";
}


Wander::~Wander()
{
}

float Wander::evaluate(Agent * agent, float dt)
{
	return 0.01f;
}

void Wander::enter(Agent * agent, float dt)
{
	glm::vec3 target = glm::vec3(0);
	if (glm::length(agent->getVelocity()) != 0)
	{
		target = agent->getPostion() + agent->getForwardDir();;
	}
	glm::vec3 displacement = glm::normalize(glm::vec3(dis(gen), 0, dis(gen)));

	target = target + displacement;

	if (m_helper > 10.f && agent->getAllAgents().size() != 0)
	{
		glm::vec3 pos = glm::vec3(0);
		for (auto& target : agent->getAllAgents())
		{
			if (target != agent)
			{
				pos = pos + target->getPostion();
			}
		}
		pos /= (agent->getAllAgents().size() - 1);

		target = target + 0.05f * (pos - target); // lerping displacement to midpoint of all other agent to avoid endless wandering (m_helper / 25.f)
	}

	generatePath(agent, target);
}

void Wander::exit(Agent * agent, float dt)
{
	if (m_helper < 20.)
		m_helper += 1.f;
	agent->setCurrentAction(nullptr);
}

void Wander::updateAction(Agent * agent, float dt)
{
	if (m_currentPath.size() > 0)
	{
		aie::Gizmos::addRing(m_currentPath.back()->position, 0.2f, 0.3f, 5, agent->getColour());
		followPath(agent, dt);
	}
	else
	{
		exit(agent, dt);
	}
}

void Wander::updateTimer(float dt)
{
	if (m_helper > 0)
	{
		m_helper -= dt;
		if (m_helper < 0)
			m_helper = 0;
	}
}

