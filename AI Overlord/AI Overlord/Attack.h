#pragma once
#include "Action.h"

// Attack is attached to agents being attacked 
class Attack : public Action
{
public:
	Attack();
	~Attack();

	
	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);

	virtual void updateTimer(float dt);

	void applyDamage(Agent* agent);

protected:
	float m_cost;
	float m_attackRange;
	float m_damageMultiplier;
	float m_coolDown;
	float m_CDTimer;
	float m_castTime;
	float m_castTimer;
	bool m_isCasting;

	//Action* 

	float checkCoolDown(Agent* agent);
	float checkMana(Agent* agent);
	float checkDPS(Agent* agent);

};

