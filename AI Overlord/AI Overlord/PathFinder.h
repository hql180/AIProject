#pragma once

#include <list>
#include "PathGraph.h"

class PathFinder
{
public:
	PathFinder();
	~PathFinder();

	static void aStar(Path::Node* startNode, Path::Node* endNode, std::list<Path::Node*> &outPath);

private:

	struct Node
	{
		Path::Node* pNode;

		Node* pParent;
		float gScore, hScore, fScore;

		Node(Path::Node* node) : pNode(node), pParent(nullptr), gScore(0) {}
	};
};

