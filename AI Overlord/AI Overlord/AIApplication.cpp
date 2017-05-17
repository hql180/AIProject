#include "AIApplication.h"


#include <list>
#include <algorithm>
#include <random>

#include "Gizmos.h"
#include "Input.h"
#include "PathFinder.h"
#include "PathGraph.h"
#include "Agent.h"
#include "Wander.h"
#include "BasicMelee.h"
#include "BasicMagic.h"
#include "BasicRange.h"
#include "Flee.h"

using namespace aie;
using namespace glm;


std::random_device rd2;
std::mt19937 gen2(rd2());
std::uniform_int_distribution<int> dis2(0, 40);

AIApplication::AIApplication() : m_pathGraph(nullptr)
{
}

AIApplication::~AIApplication()
{
}

bool AIApplication::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(100000, 100000, 100000, 100000);

	m_windowWidth = (float)getWindowWidth();
	m_windowHeight = (float)getWindowHeight();

	m_pathGraph = new PathGraph();

	for (int x = 0; x <= 40; ++x)
	{
		for (int z = 0; z <= 40; ++z)
		{
			m_pathGraph->addNode(vec3(x, 0, z));
		}
	}

	for (int x = 0; x < 40; ++x)
	{
		m_obstacles.push_back(Obstacle());
		//m_obstacles.back().position = vec3(dis2(gen2), 0, dis2(gen2));
		m_obstacles.back().position = vec3(20, 0, x);
		m_obstacles.back().shape = Shape::Box;
		m_obstacles.back().extent = vec3(1);
		m_obstacles.back().radius = 0.5f;
		m_pathGraph->removeNodeAt(m_obstacles.back().position, 0.5);
	}

	for (int x = 0; x < 40; ++x)
	{
		m_obstacles.push_back(Obstacle());
		//m_obstacles.back().position = vec3(dis2(gen2), 0, dis2(gen2));
		m_obstacles.back().position = vec3(x, 0, 20);
		m_obstacles.back().shape = Shape::Box;
		m_obstacles.back().extent = vec3(1);
		m_obstacles.back().radius = 0.5f;
		m_pathGraph->removeNodeAt(m_obstacles.back().position, 0.5);
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

	for (int i = 0; i <= 5; ++i)
	{
		m_agents.push_back(new Agent(vec3(i % 3, 0, i % 7), m_pathGraph, vec4(i % 2, i % 3, i % 4, 1)));

		m_agents.back()->setObstacle(&m_obstacles);

		m_agents.back()->getActions().push_back(new Wander());

		m_agents.back()->getActions().push_back(new Flee());

		m_agents.back()->getTActions().push_back(new BasicMelee());

		m_agents.back()->getTActions().push_back(new BasicMagic());

		m_agents.back()->getTActions().push_back(new BasicRange());

	}


	/*m_agents.push_back(new Agent(vec3(0), m_pathGraph, vec4(1, 0, 0, 1)));

	m_agents.back()->getActions().push_back(new Wander());

	m_agents.back()->getActions().push_back(new Flee());

	m_agents.back()->getTActions().push_back(new BasicMelee());

	m_agents.back()->getTActions().push_back(new BasicMagic());

	m_agents.back()->getTActions().push_back(new BasicRange());

	m_agents.push_back(new Agent(vec3(3, 0, 6), m_pathGraph, vec4(1, 1, 0, 1)));

	m_agents.back()->getActions().push_back(new Wander());

	m_agents.back()->getActions().push_back(new Flee());

	m_agents.back()->getTActions().push_back(new BasicMelee());

	m_agents.back()->getTActions().push_back(new BasicMagic());

	m_agents.back()->getTActions().push_back(new BasicRange());

	m_agents.push_back(new Agent(vec3(2, 0, 8), m_pathGraph, vec4(1, 0, 1, 1)));

	m_agents.back()->getActions().push_back(new Wander());

	m_agents.back()->getActions().push_back(new Flee());

	m_agents.back()->getTActions().push_back(new BasicMelee());

	m_agents.back()->getTActions().push_back(new BasicMagic());

	m_agents.back()->getTActions().push_back(new BasicRange());

	m_agents.push_back(new Agent(vec3(6, 0, 2), m_pathGraph, vec4(1, 1, 1, 1)));

	m_agents.back()->getActions().push_back(new Wander());

	m_agents.back()->getActions().push_back(new Flee());

	m_agents.back()->getTActions().push_back(new BasicMelee());

	m_agents.back()->getTActions().push_back(new BasicMagic());

	m_agents.back()->getTActions().push_back(new BasicRange());

	m_agents.push_back(new Agent(vec3(7, 0, 3), m_pathGraph, vec4(0, 1, 1, 1)));

	m_agents.back()->getActions().push_back(new Wander());

	m_agents.back()->getActions().push_back(new Flee());

	m_agents.back()->getTActions().push_back(new BasicMelee());

	m_agents.back()->getTActions().push_back(new BasicMagic());

	m_agents.back()->getTActions().push_back(new BasicRange());*/

	return true;
}

void AIApplication::shutdown()
{
	Gizmos::destroy();

	if (m_pathGraph)
		delete m_pathGraph;
}

void AIApplication::update(float dt)
{
	if (dt > 1.f)
		dt = 1.f;

	m_camera.update();

	Gizmos::clear();
	vec4 white(1);
	vec4 green(0, 1, 0, 1);
	vec4 black(0, 0, 0, 1);

	float speed = 2.5f;

	for (auto& obstacle : m_obstacles)
	{
		Gizmos::addAABB(obstacle.position, obstacle.extent, vec4(0.9f));
	}

	for (int i = 0; i < m_agents.size(); ++i)
	{
		m_agents[i]->update(m_agents, dt);
		Gizmos::addAABB(m_agents[i]->getPostion(), vec3(m_agents[i]->getRadius()), m_agents[i]->getColour());
	}
	
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

void AIApplication::draw()
{
	clearScreen();

	m_windowWidth = (float)getWindowWidth();
	m_windowHeight = (float)getWindowHeight();

	m_projectionMatrix = m_camera.getProjectionMatrix(m_windowWidth, m_windowHeight);

	m_viewMatrix = m_camera.getViewMatrix();

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

}
