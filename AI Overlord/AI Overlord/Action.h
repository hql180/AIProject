#pragma once

class Agent;
class Action
{
public:
	Action();
	virtual ~Action();

	virtual float evaluate(Agent* agent) = 0;
	virtual void updateAction(Agent* agent) = 0;
	virtual void enter(Agent* agent) = 0;
	virtual void exit(Agent* agent) = 0;
};

