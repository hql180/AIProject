#pragma once
#include "Action.h"
class Flee :
	public Action
{
public:
	Flee();
	~Flee();

	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);
	virtual void updateTimer(float dt);

protected:
	float m_corneredCounter;
};

