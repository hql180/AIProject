#pragma once
#include "Attack.h"
#include "Fountain.h"

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

private:
	Fountain* m_closestFountain;
	float m_currentDistance;
};

