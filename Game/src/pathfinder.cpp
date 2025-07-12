#include "precomp.h"

#include "tilemap.h"
#include "arrayList.h"
#include "level.h"

#include "pathfinder.h"

/*
	Based on multiple resources I used:

	https://mat.uab.cat/~alseda/MasterOpt/AStar-Algorithm.pdf
	https://medium.com/@nicholas.w.swift/easy-a-star-pathfinding-7e6689c7f7b2
	https://www.geeksforgeeks.org/a-search-algorithm-in-python/

*/
ArrayList<int2> Pathfinder::CalculatePath(const Level& level, int2 startPosition, int2 endPosition) const
{
	ArrayList<Node> openList;
	ArrayList<Node> closedList;

	
	openList.append(Node(startPosition, { 0,0 }, 0.f, 0.f));




	while (openList.size() > 0)
	{
		// get the smallest f value

		float currentF = openList.at(0).f;
		int currentNodeIndex = 0;
		

		for (uint i = 0; i < openList.size(); i++)
		{
			if (openList.at(i).f < currentF)
			{
				currentF = openList.at(i).f;
				currentNodeIndex = i;
			}
		}


		Node currentNode = openList.remove_at(currentNodeIndex);
		closedList.append(currentNode);

		
		if (currentNode.position.x == endPosition.x && currentNode.position.y == endPosition.y)
		{
			ArrayList<int2> reversedPath;
			ArrayList<int2> path;

			Node current = currentNode;
			while (current.cameFrom.x !=0 || current.cameFrom.y != 0)
			{
				reversedPath.append(current.position);

				int2 lookPosition = current.position + current.cameFrom;
				for (uint i = 0; i < closedList.size(); i++)
				{
					if (closedList.at(i) == Node(lookPosition, { 0,0 }, 0, 0))
					{
						current = closedList.at(i);
						break;
					}
					assert(i < closedList.size() - 1); // if not I think my look up is wrong
				}

			}

			for (uint i = 0; i < reversedPath.size(); i++)
			{
				path.append(reversedPath.at(reversedPath.size() - i - 1));
			}
			return path;

		}



		for (int dir = 0; dir < 4; dir++) // we only check 4 neighbours
		{
			int2 offset{ -1,0 };
			switch (dir)
			{
			case 0:
				offset = { 0,-1 };
				break;
			case 1:
				offset = { 1,0 };
				break;
			case 2:
				offset = { 0,1 };
				break;

			default:
				break;
			}

			
			int2 newPosition = currentNode.position + offset;

			if (newPosition.x < 0 || newPosition.x >= static_cast<int>(level.GetLevelSize().x) || newPosition.y < 0 || newPosition.y >= static_cast<int>(level.GetLevelSize().y))
				continue;

			if (!level.IsWalkable(level.GetColliderTile(int2(static_cast<uint>(newPosition.x), static_cast<uint>(newPosition.y)))))
				continue;

			float dist = static_cast<float>(abs(newPosition.x - endPosition.x) + abs(newPosition.y - endPosition.y));

			Node newNode = Node(newPosition, -offset, currentNode.g + 1, dist);


			bool nodeInClosed = false;
			
			for (uint i = 0; i < closedList.size(); i++)
			{
				if (closedList.at(i) == newNode)
				{
					nodeInClosed = true;
					break;
				}
			}

			if (nodeInClosed)
				continue;


			bool nodeInOpen = false;

			for (uint i = 0; i < openList.size(); i++)
			{
				Node other = openList.at(i);

				if (other == newNode)
				{
					if (newNode.g < other.g)
					{
						openList.at(i).cameFrom = newNode.cameFrom;
						openList.at(i).g = newNode.g;
						openList.at(i).f = newNode.h + newNode.g;

					}
					nodeInOpen = true;
					break;
				}
			}

			if (nodeInOpen)
				continue;

			openList.append(newNode);



		}




	}




	return ArrayList<int2>();
}

ArrayList<int2> Pathfinder::CalculateClosestPath(const Level& level, int2 startPosition, int2 endPosition) const
{
	ArrayList<Node> openList;
	ArrayList<Node> closedList;

	float dist = static_cast<float>(abs(startPosition.x - endPosition.x) + abs(startPosition.y - endPosition.y));

	openList.append(Node(startPosition, { 0,0 }, 0.f, dist));




	while (openList.size() > 0)
	{
		// get the smallest f value

		float currentF = openList.at(0).f;
		int currentNodeIndex = 0;


		for (uint i = 0; i < openList.size(); i++)
		{
			if (openList.at(i).f < currentF)
			{
				currentF = openList.at(i).f;
				currentNodeIndex = i;
			}
		}


		Node currentNode = openList.remove_at(currentNodeIndex);
		closedList.append(currentNode);


		if (currentNode.position.x == endPosition.x && currentNode.position.y == endPosition.y)
		{
			ArrayList<int2> reversedPath;
			ArrayList<int2> path;

			Node current = currentNode;
			while (current.cameFrom.x != 0 || current.cameFrom.y != 0)
			{
				reversedPath.append(current.position);

				int2 lookPosition = current.position + current.cameFrom;
				for (uint i = 0; i < closedList.size(); i++)
				{
					if (closedList.at(i) == Node(lookPosition, { 0,0 }, 0, 0))
					{
						current = closedList.at(i);
						break;
					}
					assert(i < closedList.size() - 1); // if not I think my look up is wrong
				}

			}

			for (uint i = 0; i < reversedPath.size(); i++)
			{
				path.append(reversedPath.at(reversedPath.size() - i - 1));
			}
			return path;

		}



		for (int dir = 0; dir < 4; dir++) // we only check 4 neighbours
		{
			int2 offset{ -1,0 };
			switch (dir)
			{
			case 0:
				offset = { 0,-1 };
				break;
			case 1:
				offset = { 1,0 };
				break;
			case 2:
				offset = { 0,1 };
				break;

			default:
				break;
			}


			int2 newPosition = currentNode.position + offset;

			if (newPosition.x < 0 || newPosition.x >= static_cast<int>(level.GetLevelSize().x) || newPosition.y < 0 || newPosition.y >= static_cast<int>(level.GetLevelSize().y))
				continue;

			if (level.GetColliderTile(int2(static_cast<int>(newPosition.x), static_cast<int>(newPosition.y))) != CollisionTileType::WALKABLE_OPEN)
				continue;

			dist = static_cast<float>(abs(newPosition.x - endPosition.x) + abs(newPosition.y - endPosition.y));

			Node newNode = Node(newPosition, -offset, currentNode.g + 1, dist);


			bool nodeInClosed = false;

			for (uint i = 0; i < closedList.size(); i++)
			{
				if (closedList.at(i) == newNode)
				{
					nodeInClosed = true;
					break;
				}
			}

			if (nodeInClosed)
				continue;


			bool nodeInOpen = false;

			for (uint i = 0; i < openList.size(); i++)
			{
				Node other = openList.at(i);

				if (other == newNode)
				{
					if (newNode.g < other.g)
					{
						openList.at(i).cameFrom = newNode.cameFrom;
						openList.at(i).g = newNode.g;
						openList.at(i).f = newNode.h + newNode.g;

					}
					nodeInOpen = true;
					break;
				}
			}

			if (nodeInOpen)
				continue;

			openList.append(newNode);
		}
	}


	// built to closest path
	


	ArrayList<int2> reversedPath;
	ArrayList<int2> path;

	Node current = closedList.at(0);
	float lowestG = LARGE_FLOAT;
	for (uint i = 0; i < closedList.size(); i++)
	{
		if (closedList.at(i).g < lowestG)
		{
			lowestG = closedList.at(i).f;
			current = closedList.at(i);
		}

	}



	while (current.cameFrom.x != 0 || current.cameFrom.y != 0)
	{
		reversedPath.append(current.position);

		int2 lookPosition = current.position + current.cameFrom;
		for (uint i = 0; i < closedList.size(); i++)
		{
			if (closedList.at(i) == Node(lookPosition, { 0 ,0}, 0, 0))
			{
				current = closedList.at(i);
				break;
			}
			assert(i < closedList.size() - 1); // if not I think my look up is wrong
		}

	}

	for (uint i = 0; i < reversedPath.size(); i++)
	{
		path.append(reversedPath.at(reversedPath.size() - i - 1));
	}
	return path;


}