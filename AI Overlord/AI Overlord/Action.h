#pragma once

class Agent;
class Action
{
public:
	virtual float evaluate(Agent* agent, float dt) = 0;
	virtual void enter(Agent* agent, float dt) = 0;
	virtual void exit(Agent* agent, float dt) = 0;
	virtual void updateAction(Agent* agent, float dt) = 0;
	virtual void updateTimer(float dt) = 0; 
};

