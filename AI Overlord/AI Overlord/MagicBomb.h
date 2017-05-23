
#include "Attack.h"
class MagicBomb :
	public Attack
{
public:
	MagicBomb();
	~MagicBomb();

	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);
	virtual void updateTimer(float dt);

protected:
	std::vector<Projectile> m_projectTilePool;

	bool m_bombReleased;

	float m_baseDamage;
};
