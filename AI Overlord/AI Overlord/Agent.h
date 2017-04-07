#pragma once
#include <vector>

#include "glm.hpp"
#include "Action.h"

struct Stats
{
	float strength = 0.0f;
	float agility = 0.0f;
	float intelligence = 0.0f;
	float vitality = 0.0f;
};

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

	float getCurrentHealth();

	float getHealthPercentage();

	float getAttackDamage();

	float getManaPercentage();

	void setAttackTarget(Agent* agent);

	std::vector<Action> getAttackList();



protected:
	glm::vec3 m_postion;
	glm::vec3 m_rotation;

	Stats m_stats;

	float maxHealth, currentHealth;

	float m_visionRange;

	std::vector<Agent*> m_actionableHostiles;

	std::vector<Action> m_actions; // list of actions for self use
	std::vector<Action> m_attackList;
	std::vector<Action> m_hostileActions; // list of actions for other agents to use on self

	Action* m_currentAction;
	float m_currentActionScore;

};

