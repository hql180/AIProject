#pragma once
#include "Attack.h"
class GoToFountain :
	public Attack
{
public:
	GoToFountain();
	~GoToFountain();

	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);
};

