#pragma once
#include "Attack.h"
class Charge :
	public Attack
{
public:
	Charge();
	~Charge();

	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);
	virtual void updateTimer(float dt);
};

