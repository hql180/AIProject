#pragma once
#include "Action.h"

// Attack is attached to agents being attacked 
class Attack : public Action
{
public:
	Attack(Agent* agent);
	~Attack();

	// Agent here is the attacker
	virtual float evaluate(Agent* agent);
	virtual void enter(Agent* agent);
	virtual void exit(Agent* agent);
	virtual void updateAction(Agent* agent);

protected:
	// Pointer to agent this attack action is placed on
	Agent* m_targetAgent;
};

