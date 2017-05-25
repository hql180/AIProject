#pragma once
#include <vector>

#include "glm.hpp"
#include "Action.h"
#include "Attack.h"
#include "PathGraph.h"
#include "Obstacle.h"
#include "Fountain.h"

struct Stats
{
	float strength = 5.0f;
	float agility = 5.0f;
	float intelligence = 5.0f;
	float vitality = 10.0f;
};

class Agent
{
public:
	Agent();
	Agent(glm::vec3& pos, PathGraph* graph, std::vector<Obstacle>* obstacles, std::vector<Fountain>* fountains, 
			glm::vec4& colour, float FOV = 100.f, float visionRange = 15.f, float radius = 1.f, glm::vec3& forwardDir = glm::vec3(1, 0, 0));
	~Agent();

	void setUp(Stats& stats, float moveSpeed = 1.f, float preferedRange = 0.f);



	glm::vec3 getPostion();

	void setPosition(glm::vec3& position);

	bool contains(std::vector<Agent*> list, Agent* agent);

	bool inVisionRange(Agent* agent);

	bool inLineOfSight(Agent* target);

	bool inFOV(float angle);

	Action* getBestAction(float dt); 

	Action* getCurrentAction();

	void update(std::vector<Agent*> agentList, float dt);

	float getCurrentHealth();

	float getHealthPercentage();

	void addHealth(float amount);

	float getAttackDamage();

	bool takeDamage(float damage);

	void subMana(float amount);

	void addMana(float amount);

	float getCurrentMana();

	float getManaPercentage();

	float getAngleToTarget(Agent* target);

	float getMoveSpeed();

	void setMoveSpeed(float speed);

	void updateMovement(float dt);

	void setCurrentAction(Action* action);

	void setTarget(Agent* agent);

	void respawn(Agent* agent, float dt);

	glm::vec3 getVelocity();

	void setVelocity(glm::vec3& targetVelocity);

	Agent* getTarget();

	//temporary fix
	Action* getEngage();

	PathGraph* getPathGraph();

	std::vector<Action*>& getHActions();

	std::vector<Action*>& getTActions();

	std::vector<Action*>& getActions();

	std::vector<Agent*>& getHostiles();

	std::vector<Agent*>& getAllAgents();

	glm::vec4& getColour();

	glm::vec3 getDirectionToTarget();

	glm::vec3 getDirectionToTarget(Agent* from, Agent* to);

	glm::vec3 getForwardDir();

	Stats& getStats();

	float getRadius();

	float getVisionRange();

	float getPreferedRange();

	void setObstacle(std::vector<Obstacle>* obs);

	std::vector<Obstacle>* getObstacle();

	void setFountains(std::vector<Fountain>* fountains);

	std::vector<Fountain>* getFountains();

	int getStatPoints();

	void setStatPoints(int value);

	void updateStats();

	bool player;

protected:
	glm::vec3 m_postion;
	glm::vec3 m_velocity;
	glm::vec3 m_dir;
	glm::vec4 m_colour;
	Stats m_stats;

	int m_statPoints;

	float m_radius;

	float m_moveSpeed;

	float m_maxHealth, m_health;

	float m_maxMana, m_mana;

	float m_preferedRange;

	float m_visionRange, m_FOV;

	float m_showDamage;

	bool m_isDead;

	Agent* m_currentTarget;

	Action* m_currentAction;

	float m_currentActionScore;

	PathGraph* m_pathGraph;

	std::vector<Agent*> m_actionableHostiles;
	std::vector<Agent*> m_allAgents;

	std::vector<Action*> m_actions; // list of actions for self use
	std::vector<Action*> m_targetedActions;
	std::vector<Action*> m_hostileActions; // list of actions for other agents to use on self

	std::vector<Obstacle>* m_obstacles;
	std::vector<Fountain>* m_fountains;
};

