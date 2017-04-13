#pragma once

#include <vector>
#include "glm.hpp"



namespace Path
{
	struct Edge;

	struct Node
	{
		glm::vec3 position;

		//float gScore;

		//Node* parent;

		std::vector<Edge> connections;

		Node() : position(0.0f)  {} //parent(nullptr) gScore(0)

		Node(glm::vec3 pos) : position(pos) {}

		//Node(glm::vec3 pos, float score, Node* pParent) : position(pos), gScore(score), parent(pParent) {}
	};

	struct Edge
	{
		// pointer to connected node
		Node* connectedNode;

		float cost;

		Edge() : connectedNode(nullptr), cost(0) {}

		Edge(Node* pConnection) : connectedNode(pConnection), cost(0) {}

		Edge(Node* pConnection, float connectionCost) : connectedNode(pConnection), cost(connectionCost) {}
	};

}

using namespace Path;

class PathGraph
{
public:
	PathGraph();
	~PathGraph();

	Node* addNode(glm::vec3 position); // Adds a path node to nodeList

	// Adds connection between src and dest
	// Cost being desitance between src and dest if not given a value
	void addConnection(Node* src, Node* dest, float cost = -1.0f);

	// finds all nodes within radius of given pos and outputs into vector provided
	void getNodesInRadius(std::vector<Node*> &outNodes, glm::vec3 pos, float radius);

	// Returns a node at given position
	Node* findNode(glm::vec3 pos, float radius);

	// Removes node at given pos or removes nodes within radius of postion
	void removeNodeAt(glm::vec3 pos, float radius = 0.0f);

	std::vector<Node*>& getNodeList();

private:
	std::vector<Node*> m_nodeList;
};

