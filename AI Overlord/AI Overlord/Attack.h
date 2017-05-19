#pragma once
#include "Action.h"

// Attack is attached to agents being attacked 
class Attack : public Action
{
	friend class Projectile;
public:
	Attack();
	~Attack();

	
	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);

	virtual void updateTimer(float dt);

	void applyDamage(Agent* target, float damage);

	void finishAttack(Agent* agent);

	void seekTarget(Agent* agent, float dt);

protected:	
	bool needNewPath(Agent* agent);

	float m_cost;
	float m_attackRange;
	float m_radius;
	float m_projectileSpeed;
	float m_damageMultiplier;
	float m_CD;
	float m_CDTimer;
	float m_castTime;
	float m_castTimer;
	bool m_isCasting;

	//Action* 

	float checkCoolDown(Agent* agent);
	float checkMana(Agent* agent);
	float checkDamage(Agent* agent);

};

