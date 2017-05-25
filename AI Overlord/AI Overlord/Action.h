#pragma once
#include <list>
#include "glm.hpp"
#include "PathGraph.h"

class Agent;
class Action
{
public:
	virtual float evaluate(Agent* agent, float dt) = 0;
	virtual void enter(Agent* agent, float dt) = 0;
	virtual void exit(Agent* agent, float dt) = 0;
	virtual void updateAction(Agent* agent, float dt) = 0;
	virtual void updateTimer(float dt);

	char *name;
protected:
	void generatePath(Agent* agent, glm::vec3& destination);

	void followPath(Agent* agent, float dt);

	std::list<Node*> m_currentPath;
};

