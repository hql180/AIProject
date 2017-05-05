#include "Move.h"
#include "PathFinder.h"
#include "Agent.h"
#include "Gizmos.h"

Move::Move()
{
}


Move::~Move()
{
}

float Move::evaluate(Agent * agent, float dt)
{
	return 0.0f;
}

void Move::enter(Agent * agent, float dt)
{
}

void Move::exit(Agent * agent, float dt)
{
}

void Move::updateAction(Agent * agent, float dt)
{
}

void Move::updateTimer(float dt)
{
}

void Move::generatePath(Agent * agent, glm::vec3 & destination)
{
	Node* targetNode = nullptr;
	Node* currentNode = nullptr;

	float radius = 1.0f;

	while (targetNode == nullptr)
	{
		targetNode = agent->getPathGraph()->findNode(destination, 0.3f * radius++);
	}
	
	radius = 1.0f;

	while (currentNode == nullptr)
	{
		currentNode = agent->getPathGraph()->findNode(agent->getPostion(), 0.3f * radius++);
	}
	m_currentPath.clear();

	PathFinder::aStar(currentNode, targetNode, m_currentPath);
}

void Move::followPath(Agent * agent, float dt)
{
	glm::vec3 vecToTarget = (m_currentPath.front()->position - agent->getPostion());

	aie::Gizmos::addRing(m_currentPath.front()->position, 0.2f, 0.3f, 5, agent->getColour());

	glm::vec3 force = ((glm::length(vecToTarget)!= 0) ? glm::normalize(vecToTarget) : vecToTarget) * agent->getMoveSpeed();

	if (glm::length(vecToTarget) < 2.5f)
		m_currentPath.pop_front();

	agent->setVelocity(force);
	

	agent->updateMovement(dt);
}
