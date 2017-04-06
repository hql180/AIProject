#include "AIOverlord.h"

#include "Gizmos.h"

using namespace aie;
using namespace glm;


AIOverlord::AIOverlord()
{
}

AIOverlord::~AIOverlord()
{
}

bool AIOverlord::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	m_windowWidth = (float)getWindowWidth();
	m_windowHeight = (float)getWindowHeight();

	return true;
}

void AIOverlord::shutdown()
{
	Gizmos::destroy();
}

void AIOverlord::update(float dt)
{
	m_camera.update();

	Gizmos::clear();
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i) 
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}
}

void AIOverlord::draw()
{
	clearScreen();

	m_windowWidth = (float)getWindowWidth();
	m_windowHeight = (float)getWindowHeight();

	m_projectionMatrix = m_camera.getProjectionMatrix(m_windowWidth, m_windowHeight);

	m_viewMatrix = m_camera.getViewMatrix();

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

}
