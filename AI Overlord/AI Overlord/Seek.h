#pragma once
#include "Move.h"
class Seek :
	public Move
{
public:
	Seek();
	~Seek();

	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);
	virtual void updateTimer(float dt);
};

