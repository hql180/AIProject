#include "AIOverlord.h"

#include <list>
#include <algorithm>


#include "Gizmos.h"
#include "Input.h"
#include "PathFinder.h"
#include "PathGraph.h"


using namespace aie;
using namespace glm;


AIOverlord::AIOverlord() : m_pathGraph(nullptr)
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

	m_pathGraph = new PathGraph();

	for (int x = 0; x <= 10; ++x)
	{
		for (int z = 0; z <= 10; ++z)
		{
			m_pathGraph->addNode(vec3(x, 0, z));
		}
	}

	for (auto& node : m_pathGraph->getNodeList())
	{
		std::vector<Node*> nodesInRadius;
		m_pathGraph->getNodesInRadius(nodesInRadius, node->position, 1.1f);
		for (auto& nodeToConnect : nodesInRadius)
		{
			if (nodeToConnect != node)
			{
				m_pathGraph->addConnection(node, nodeToConnect);
			}
		}
	}

	m_testGizmo = vec3(5, 0, 5);

	m_chaseGizmo = vec3(3, 0, 3);

	return true;
}

void AIOverlord::shutdown()
{
	Gizmos::destroy();

	if (m_pathGraph)
		delete m_pathGraph;
}

void AIOverlord::update(float dt)
{
	m_camera.update();

	Gizmos::clear();
	vec4 white(1);
	vec4 green(0, 1, 0, 1);
	vec4 black(0, 0, 0, 1);

	float speed = 2.5f;


	if (aie::Input::getInstance()->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		m_testGizmo.z -= speed * dt;
	}
	else if (aie::Input::getInstance()->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		m_testGizmo.z += speed * dt;
	}

	if (aie::Input::getInstance()->isKeyDown(aie::INPUT_KEY_UP))
	{
		m_testGizmo.x += speed * dt;
	}
	else if (aie::Input::getInstance()->isKeyDown(aie::INPUT_KEY_DOWN))
	{
		m_testGizmo.x -= speed * dt;
	}
	
	if (aie::Input::getInstance()->isKeyDown(aie::INPUT_KEY_F))
	{
		m_pathGraph->removeNodeAt(m_testGizmo, 0.5f);
	}

	if (aie::Input::getInstance()->wasKeyPressed(aie::INPUT_KEY_G))
	{
		std::vector<Node*> nearbyConnections;

		m_pathGraph->getNodesInRadius(nearbyConnections, m_testGizmo, 1.1f);

		Node* node = m_pathGraph->addNode(m_testGizmo);		

		for (auto& nodeToAdd : nearbyConnections)
		{
			m_pathGraph->addConnection(node, nodeToAdd);
			m_pathGraph->addConnection(nodeToAdd, node);
		}
	}

	std::list<Node*> path;

	if (m_pathGraph->findNode(m_testGizmo, 0.6f) && m_pathGraph->findNode(m_chaseGizmo, 1.0f))
		PathFinder::aStar(m_pathGraph->findNode(m_chaseGizmo, 0.6f), m_pathGraph->findNode(m_testGizmo, 1.0f), path);

	if (path.size() > 1)
	{
		path.pop_front();

		m_chaseGizmo += (glm::normalize(path.front()->position - m_chaseGizmo) * 1.5f * dt);
	}

	Gizmos::addAABB(m_chaseGizmo, vec3(0.5f, 0.5f, 0.5f), vec4(1, 0, 0, 1));
	Gizmos::addAABB(m_testGizmo, vec3(0.5f), vec4(0, 0, 1, 1));

	Node* current = nullptr;
	Node* previous = nullptr;


	for (auto& node : path)
	{
		current = node;

		if (previous)
		{
			Gizmos::addLine(previous->position, current->position, green);
		}

		previous = current;
	}

	for (auto& node : m_pathGraph->getNodeList())
	{
		Gizmos::addSphere(node->position, 0.1f, 1, 1, white);
		for (auto& edgeConnection : node->connections)
		{
			Gizmos::addLine(node->position, edgeConnection.connectedNode->position, black);
		}
	}

	//for (int i = 0; i < 21; ++i) 
	//{
	//	Gizmos::addLine(vec3(-10 + i, 0, 10),
	//		vec3(-10 + i, 0, -10),
	//		i == 10 ? white : black);
	//	Gizmos::addLine(vec3(10, 0, -10 + i),
	//		vec3(-10, 0, -10 + i),
	//		i == 10 ? white : black);
	//}
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
