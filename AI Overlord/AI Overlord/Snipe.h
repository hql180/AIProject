#pragma once
#include "Attack.h"
class Snipe :
	public Attack
{
public:
	Snipe();
	~Snipe();

	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);
	virtual void updateTimer(float dt);

protected:
	std::vector<Projectile> m_projectTilePool;
};

