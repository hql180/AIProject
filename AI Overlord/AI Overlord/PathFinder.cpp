#include "PathFinder.h"



PathFinder::PathFinder()
{
}


PathFinder::~PathFinder()
{
}

void PathFinder::aStar(Path::Node * startNode, Path::Node * endNode, std::list<Path::Node*>& outPath)
{
	std::list<Node*> openList;
	std::list<Node*> closedList;

	openList.push_back(new Node(startNode));

	Node* current;

	while (!openList.empty())
	{
		openList.sort([](Node* a, Node* b) { return a->fScore < b->fScore; });

		current = openList.front();

		if (current->pNode == endNode)
			break;

		closedList.push_front(current);
		openList.pop_front();

		for (auto const& edgeConnection : current->pNode->connections)
		{
			bool isAdded = false;

			float gScore = current->gScore + edgeConnection.cost;

			for (auto& node : closedList)
			{
				if (node->pNode == edgeConnection.connectedNode)
				{
					isAdded = true;
					if (node->gScore > gScore)
					{
						node->gScore = gScore;
						node->fScore = node->gScore + node->hScore;
						node->pParent = current;
					}
				}
			}
		}
	}
}
