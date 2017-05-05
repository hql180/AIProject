#pragma once
#include "Action.h"
#include "PathGraph.h"
#include <list>

class Move :
	public Action
{
public:
	Move();
	~Move();

	virtual float evaluate(Agent* agent, float dt);
	virtual void enter(Agent* agent, float dt);
	virtual void exit(Agent* agent, float dt);
	virtual void updateAction(Agent* agent, float dt);
	virtual void updateTimer(float dt);

protected:
	void generatePath(Agent* agent, glm::vec3& destination);

	void followPath(Agent* agent, float dt);

	std::list<Node*> m_currentPath;
};

