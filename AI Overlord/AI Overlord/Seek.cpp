#include "Seek.h"
#include "Agent.h"
#include "Gizmos.h"

Seek::Seek()
{
}


Seek::~Seek()
{
}

float Seek::evaluate(Agent * agent, float dt)
{
	return 2.0f;
}

void Seek::enter(Agent * agent, float dt)
{
	generatePath(agent, agent->getAttackTarget()->getPostion());
	if (m_currentPath.size() > 0)
		m_currentPath.pop_front();

	agent->setCurrentAction(this);
}

void Seek::exit(Agent * agent, float dt)
{
	agent->setCurrentAction(agent->getAttackTarget()->getEngage());
}

void Seek::updateAction(Agent * agent, float dt)
{
	if (m_currentPath.size() > 0)
	{
		followPath(agent, dt);
		aie::Gizmos::addRing(agent->getAttackTarget()->getPostion(), 0.2f, 0.3f, 5, agent->getColour());
	}
	
	exit(agent, dt);
}

void Seek::updateTimer(float dt)
{
}
