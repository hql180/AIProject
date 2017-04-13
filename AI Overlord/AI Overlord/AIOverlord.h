#pragma once
#include "Application.h"
#include "glm.hpp"
#include "Camera.h"

class PathGraph;

class AIOverlord : public aie::Application
{
public:
	AIOverlord();
	virtual ~AIOverlord();

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

	glm::vec3 m_testGizmo;
	glm::vec3 m_chaseGizmo;

};

