#pragma once
#include "Action.h"
class Wander :
	public Action
{
public:
	Wander();
	~Wander();

	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);
};

