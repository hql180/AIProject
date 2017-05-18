#pragma once
#include "Attack.h"
class Heal :
	public Attack
{
public:
	Heal();
	~Heal();

	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);
	virtual void updateTimer(float dt);
};

