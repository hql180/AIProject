#pragma once
#include "Application.h"
#include "glm.hpp"
#include "Camera.h"
#include "Obstacle.h"
#include "Fountain.h"
#include <vector>

class PathGraph;
class Agent;



class AIApplication : public aie::Application
{
public:
	AIApplication();
	virtual ~AIApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float dt);
	virtual void draw();

	void applySkillSet(Agent* agent, int setID);

	void restart();

	Camera m_camera;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;

	float m_windowWidth, m_windowHeight;

	PathGraph* m_pathGraph;

	bool m_start;

	std::vector<Obstacle> m_obstacles;
	std::vector<Fountain> m_fountains;
	
	std::vector<Agent*> m_agents;

	Agent* m_agent;
};

