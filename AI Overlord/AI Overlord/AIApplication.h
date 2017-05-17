#pragma once
#include "Application.h"
#include "glm.hpp"
#include "Camera.h"
#include "Obstacle.h"
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

private:
	Camera m_camera;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;

	float m_windowWidth, m_windowHeight;

	PathGraph* m_pathGraph;

	std::vector<Obstacle> m_obstacles;

	glm::vec3 m_testGizmo;
	glm::vec3 m_chaseGizmo;
	
	std::vector<Agent*> m_agents;

};

