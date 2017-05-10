#pragma once
#include <vector>

#include "glm.hpp"
#include "Action.h"
#include "Attack.h"
#include "PathGraph.h"

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
	Agent(glm::vec3& pos, PathGraph* graph, glm::vec4& colour);
	~Agent();

	glm::vec3 getPostion();

	void setPosition(glm::vec3& position);

	bool contains(std::vector<Agent*> list, Agent* agent);

	bool inVisionRange(Agent* agent);

	bool inLineOfSight(Agent* target);

	Action* getBestAction(float dt); 

	void update(std::vector<Agent*> agentList, float dt);

	float getCurrentHealth();

	float getHealthPercentage();

	float getAttackDamage();

	void subMana(float amount);

	float getCurrentMana();

	float getManaPercentage();

	float getDistanceToTarget();

	void checkDistanceToTarget();

	float getAngleToTarget(Agent* target);

	float getMoveSpeed();

	void updateMovement(float dt);

	void setCurrentAction(Action* action);

	void setTarget(Agent* agent);

	bool takeDamage(float damage);

	glm::vec3 getVelocity();

	void setVelocity(glm::vec3& targetVelocity);

	Agent* getTarget();

	//temporary fix
	Action* getEngage();

	PathGraph* getPathGraph();

	std::vector<Action*>& getHActions();

	std::vector<Action*>& getTActions();

	std::vector<Action*>& getActions();

	glm::vec4& getColour();

protected:
	glm::vec3 m_postion;
	glm::vec3 m_velocity;
	glm::vec3 m_rotation;

	glm::vec4 m_colour;

	Stats m_stats;

	float m_moveSpeed;

	float m_maxHealth, m_health;

	float m_maxMana, m_mana;

	float m_minSkillRange, m_maxSkillRange;

	float m_visionRange, m_FOV;

	float m_actionCD;

	float m_distanceToTarget;

	PathGraph* m_pathGraph;

	std::vector<Agent*> m_actionableHostiles;

	std::vector<Action*> m_actions; // list of actions for self use
	std::vector<Action*> m_targetedActions;
	std::vector<Action*> m_hostileActions; // list of actions for other agents to use on self

	Agent* m_currentTarget;

	Action* m_currentAction;
	float m_currentActionScore;

};

