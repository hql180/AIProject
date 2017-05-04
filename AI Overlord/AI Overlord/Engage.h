#pragma once
#include "Action.h"

// Engage is attached to agents being attacked 
class Engage :
	public Action
{
public:
	Engage();
	Engage(Agent* agent);
	~Engage();

	// Agent here is the attacker
	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);

protected:
	// Pointer to agent being attacked
	Agent* m_targetAgent;

};

