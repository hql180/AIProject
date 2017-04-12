#pragma once

#include <list>
#include "PathGraph.h"

class PathFinder
{
public:
	PathFinder();
	~PathFinder();

	void aStar(Path::Node* startNode, Path::Node* endNode, std::list<Path::Node*> &outPath);

private:

	struct Node
	{
		Path::Node* pNode;

		Node* pParent;
		float dos, gScore, hScore, fScore;

		Node(Path::Node* node) : pNode(node), pParent(nullptr), dos(0), gScore(0), hScore(0), fScore(0) {}
	};
};

