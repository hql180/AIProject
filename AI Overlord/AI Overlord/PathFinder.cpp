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

		for (auto& edgeConnection : current->pNode->connections)
		{
			bool isAdded = false;

			float gScore = current->gScore + edgeConnection.cost;

			for (auto& closedNode : closedList)
			{
				if (closedNode->pNode == edgeConnection.connectedNode)
				{
					isAdded = true;
					if (closedNode->gScore > gScore)
					{
						closedNode->gScore = gScore;
						closedNode->fScore = closedNode->gScore + closedNode->hScore;
						closedNode->pParent = current;
					}
				}
			}

			for (auto& openNode : openList)
			{
				if (openNode->pNode == edgeConnection.connectedNode)
				{
					isAdded = true;
					if (openNode->gScore > gScore)
					{
						openNode->gScore = gScore;
						openNode->fScore = openNode->gScore + openNode->hScore;
						openNode->pParent = current;
					}
				}
			}

			if (!isAdded)
			{
				openList.push_front(new Node(edgeConnection.connectedNode));
				openList.front()->gScore = current->gScore + edgeConnection.cost;
				openList.front()->hScore = glm::length(openList.front()->pNode->position - endNode->position);
				openList.front()->fScore = openList.front()->gScore + openList.front()->hScore;
				openList.front()->pParent = current;
			}

		}
	}

	if (current->pNode == endNode)
	{
		while (current)
		{
			outPath.push_front(current->pNode);
			current = current->pParent;
		}
	}

	for (auto& node : openList)
		delete node;

	for (auto& node : closedList)
		delete node;
}
