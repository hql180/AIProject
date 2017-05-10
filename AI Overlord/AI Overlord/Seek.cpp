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
	generatePath(agent, agent->getTarget()->getPostion());

	agent->setCurrentAction(this);
}

void Seek::exit(Agent * agent, float dt)
{
	agent->setCurrentAction(agent->getTarget()->getEngage());
}

void Seek::updateAction(Agent * agent, float dt)
{
	if (m_currentPath.size() > 0)
	{
		followPath(agent, dt);
		aie::Gizmos::addRing(agent->getTarget()->getPostion(), 0.2f, 0.3f, 5, agent->getColour());
	}
	else
		exit(agent, dt);
}
