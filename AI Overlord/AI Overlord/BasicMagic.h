#pragma once
#include "Attack.h"
class BasicMagic :
	public Attack
{
public:
	BasicMagic();
	~BasicMagic();

	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);
	virtual void updateTimer(float dt);
};

