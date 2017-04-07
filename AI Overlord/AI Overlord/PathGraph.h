#pragma once

#include <vector>
#include "glm.hpp"

struct Edge;


struct Node
{
	glm::vec3 position;

	float gScore;

	Node* parent;

	std::vector<Edge> connections;

	Node() : position(0.0f), gScore(0), parent(nullptr) {}

	Node(glm::vec3 pos) : position(pos) {}

	Node(glm::vec3 pos, float score, Node* pParent) : position(pos), gScore(score), parent(pParent) {}	
};

struct Edge
{
	Node* connection;

	float cost;

	Edge() : connection(nullptr), cost(0) {}

	Edge(Node* pConnection) : connection(pConnection), cost(0) {}

	Edge(Node* pConnection, float connectionCost) : connection(pConnection), cost(connectionCost) {}
};

class PathGraph
{
public:
	PathGraph();
	~PathGraph();
};

