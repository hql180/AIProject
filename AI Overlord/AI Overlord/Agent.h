#pragma once
#include <vector>

#include "glm.hpp"
#include "Action.h"

class Agent
{
public:
	Agent();
	~Agent();

	glm::vec3 getPostion();

	bool contains(std::vector<Agent*> list, Agent* agent);

	bool inVisionRange(Agent* agent);

	Action* getBestAction(); 

	void update(std::vector<Agent*> agentList);



protected:
	glm::vec3 m_postion;
	glm::vec3 m_rotation;

	float m_visionRange;

	std::vector<Agent*> m_actionableHostiles;

	std::vector<Action> m_actions; // list of actions for self use
	std::vector<Action> m_;
	std::vector<Action> m_hostileActions; // list of actions for other agents to use on self

	Action* m_currentAction;
	float m_currentActionScore;

};

