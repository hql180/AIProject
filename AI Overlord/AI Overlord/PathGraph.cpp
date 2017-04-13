#include "PathGraph.h"

PathGraph::PathGraph()
{
}


PathGraph::~PathGraph()
{
	for (auto& node : m_nodeList)
	{
		delete node;
	}
}

Node * PathGraph::addNode(glm::vec3 position)
{
	// Creating a new node and adding it to nodelist
	Node* node = new Node(position);
	m_nodeList.push_back(node);
	return node;
}

void PathGraph::addConnection(Node * src, Node * dest, float cost)
{
	src->connections.push_back(Path::Edge(dest, glm::length(src->position - dest->position)));
}

void PathGraph::getNodesInRadius(std::vector<Node*>& outNodes, glm::vec3 pos, float radius)
{
	for (auto const& node : m_nodeList)
		if (glm::length(node->position - pos) <= radius)
			outNodes.push_back(node);
}

Node * PathGraph::findNode(glm::vec3 pos, float radius)
{
	for (auto const& node : m_nodeList)
		if (glm::length(node->position - pos) <= radius)
			return node;
	return nullptr;
}

void PathGraph::removeNodeAt(glm::vec3 pos, float radius)
{
	std::vector<Node*> removalList;

	getNodesInRadius(removalList, pos, radius);

	for (auto & nodeToRemove : removalList)
	{
		// Loops through all nodes in nodelist
		for (auto & node : m_nodeList)
		{
			// Loops through all connections of each node
			for (int i = node->connections.size() - 1; i >= 0; --i)
			{
				// Removes edge connections to node being removed
				if (node->connections[i].connectedNode == nodeToRemove)
				{
					node->connections.erase(node->connections.begin() + i);
				}
			}
		}

		for (int i = m_nodeList.size() - 1; i >= 0; --i)
		{
			if (m_nodeList[i] == nodeToRemove)
			{
				m_nodeList.erase(m_nodeList.begin() + i);
				delete nodeToRemove;
			}
		}
	}
}

std::vector<Node*>& PathGraph::getNodeList()
{
	return m_nodeList;
}
