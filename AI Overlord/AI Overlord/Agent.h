#pragma once
#include <vector>

#include "glm.hpp"
#include "Action.h"
#include "Attack.h"

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

	void update(std::vector<Agent*> agentList, float dt);

	float getCurrentHealth();

	float getHealthPercentage();

	float getAttackDamage();

	void subMana(float amount);

	float getCurrentMana();

	float getManaPercentage();

	float getDistanceToTarget();

	void checkDistanceToTarget();

	float getMoveSpeed();

	void setCurrentAction(Action* action);

	void setAttackTarget(Agent* agent);

	bool takeDamage(float damage);

	Agent* getAttackTarget();

	//temporary fix
	Action* getEngage();

	std::vector<Action> getAttackList();



protected:
	glm::vec3 m_postion;
	glm::vec3 m_rotation;

	Stats m_stats;

	float m_maxHealth, m_health;

	float m_maxMana, m_mana;

	float m_minSkillRange, m_maxSkillRange;

	float m_visionRange;

	float m_actionCD;

	float m_distanceToTarget;

	std::vector<Agent*> m_actionableHostiles;

	std::vector<Action> m_actions; // list of actions for self use
	std::vector<Attack> m_attackList;
	std::vector<Action> m_hostileActions; // list of actions for other agents to use on self

	Agent* m_currentTarget;

	Action* m_currentAction;
	float m_currentActionScore;

};

