#pragma once

class Tilemap;

class Level;


class Pathfinder
{
public:
	Pathfinder() = default;


	ArrayList<int2> CalculatePath(const Level& level, int2 startPosition, int2 endPosition) const;
	ArrayList<int2> CalculateClosestPath(const Level& level, int2 startPosition, int2 endPosition) const;


private:
	struct Node
	{
		Node() = default;

		Node(int2 position, int2 cameFrom, float gValue, float hValue)
			: position{ position }
			, cameFrom{ cameFrom }
			, g{gValue}
			, h{hValue}
			, f{ g + h }	{}


		bool operator==(const Node& other) const
		{
			return position.x==other.position.x && position.y == other.position.y;
			
		}


		int2 position{ 0 };
		int2 cameFrom{ 0 };

		float g{ 0.f };	// distance from start
		float h{ 0.f }; // heuristics to the end
		float f{ 0.f };

	};
	
};

