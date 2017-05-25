#pragma once
#include "Attack.h"

// Specific attacks are attached to the attacking agent
class BasicMelee :
	public Attack
{
public:
	BasicMelee();
	~BasicMelee();

	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);
	virtual void updateTimer(float dt);

};


