#include "AIApplication.h"


#include <list>
#include <algorithm>
#include <random>

#include "imgui_glfw3.h"
#include "imgui\imgui.h"
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
#include "Charge.h"
#include "Heal.h"
#include "GoToFountain.h"
#include "UI.h"

using namespace aie;
using namespace glm;

int size = 40;

std::random_device rd2;
std::mt19937 gen2(rd2());
std::uniform_int_distribution<int> dis2(0, size);

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

	for (int x = 0; x <= size; ++x)
	{
		for (int z = 0; z <= size; ++z)
		{
			m_pathGraph->addNode(vec3(x, 0, z));
		}
	}

	for (int x = 0; x < 10; ++x)
	{
		m_obstacles.push_back(Obstacle());
		m_obstacles.back().position = vec3(dis2(gen2), 0, dis2(gen2));
		m_obstacles.back().shape = Shape::Box;
		if ( false && x % 5 == 0)
		{
			m_obstacles.back().extent = vec3(2.f);
			m_obstacles.back().radius = 2.f;
			m_pathGraph->removeNodeAt(m_obstacles.back().position, 2.f);
		}
		else
		{
			m_obstacles.back().extent = vec3(1.5f);
			m_obstacles.back().radius = 1.5f;
			m_pathGraph->removeNodeAt(m_obstacles.back().position, 1.5f);
		}

	}

	m_fountains.push_back(Fountain(vec3(size / 2, 0, size / 2)));

	m_pathGraph->removeNodeAt(m_fountains.back().position, m_fountains.back().radius);

	//m_fountains.push_back(Fountain(vec3(3, 0, 6)));

	//m_pathGraph->removeNodeAt(m_fountains.back().position, m_fountains.back().radius);

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


	m_agent = new Agent(vec3(20, 0, 20), m_pathGraph, &m_obstacles, &m_fountains, vec4(.3, .7, 1, 1));

	m_agents.push_back(m_agent);

	m_agents.back()->setUp(Stats());

	m_agents.back()->getActions().push_back(new Wander());

	m_agents.back()->getActions().push_back(new Flee());

	m_agents.back()->getActions().push_back(new Heal());

	m_agents.back()->getActions().push_back(new GoToFountain());

	m_agents.back()->getTActions().push_back(new BasicMelee());

	//m_agents.back()->getTActions().push_back(new BasicMagic());

	m_agents.back()->getTActions().push_back(new Charge());



	//m_agents.push_back(new Agent(vec3(8, 0, 6), m_pathGraph, vec4(0, 1, 0, 1), vec3(-1, 0, 0)));

	//m_agents.back()->setObstacle(&m_obstacles);

	////m_agents.back()->getActions().push_back(new Wander());

	//m_agents.back()->getActions().push_back(new Flee());

	//m_agents.back()->getTActions().push_back(new BasicMelee());

	//m_agents.back()->getTActions().push_back(new BasicMagic());

	//m_agents.back()->getTActions().push_back(new BasicRange());

	for (int i = 0; i < 2; ++i)
	{		
		m_agents.push_back(new Agent(vec3(i % 3, 0, i % 7), m_pathGraph, &m_obstacles, &m_fountains, vec4(i % 2, i % 3, i % 4, 1)));

		m_agents.back()->setUp(Stats(), 1, 6.f);

		m_agents.back()->getActions().push_back(new Wander());

		m_agents.back()->getActions().push_back(new Flee());

		m_agents.back()->getActions().push_back(new Heal());

		m_agents.back()->getActions().push_back(new GoToFountain());

		m_agents.back()->getTActions().push_back(new BasicMagic());

		m_agents.back()->getTActions().push_back(new BasicRange());

	}

	m_camera = Camera(vec3(size/2, size / 3, size * 1.5f), -90);

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
		
	//static bool show = true;

	////m_UI.run(&show);

	////UI::run(&show);

	Gizmos::clear();
	vec4 white(1);
	vec4 green(0, 1, 0, 1);
	vec4 black(0, 0, 0, 1);

	float speed = 2.5f;

	for (auto& fountain : m_fountains)
	{
		fountain.update(m_agents, dt);
	}

	for (auto& obstacle : m_obstacles)
	{
		glm::vec3 pos = obstacle.position;
		pos.y += obstacle.radius;
		Gizmos::addAABB(pos, obstacle.extent, vec4(0.9f));
	}

	for (int i = 0; i < m_agents.size(); ++i)
	{
		m_agents[i]->update(m_agents, dt);
		Gizmos::addAABB(m_agents[i]->getPostion(), vec3(m_agents[i]->getRadius()), m_agents[i]->getColour());
	}
	
	

	for (auto& node : m_pathGraph->getNodeList())
	{
		Gizmos::addSphere(node->position, 0.1f, 1, 1, white);
		for (auto& edgeConnection : node->connections)
		{
			Gizmos::addLine(node->position, edgeConnection.connectedNode->position, black);
		}
	}



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
