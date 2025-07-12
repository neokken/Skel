#include "precomp.h"

#include "tilesheet.h"
#include "arrayList.h"

#include "tilemap.h"

TileMap::TileMap(const char* path, uint2 _mapSize)
	: mapSize{ _mapSize }
{
	LoadCsv(path);
}

TileMap::TileMap(const TileMap& other)
{
	mapSize = other.mapSize;
	tileSheet = other.tileSheet;

	levelData = new uint[mapSize.x * mapSize.y];
	for (uint i = 0; i < mapSize.x * mapSize.y; i++)
	{
		levelData[i] = other.levelData[i];
	}
}

TileMap::TileMap(TileMap&& other) noexcept
{
	mapSize = other.mapSize;
	tileSheet = other.tileSheet;
	levelData = other.levelData;

	other.levelData = nullptr;	// don't think this is needed. 
}

TileMap::~TileMap()
{
	delete[] levelData;
}

TileMap& TileMap::operator=(const TileMap& rhs)
{
	if (this != &rhs)
	{
		mapSize = rhs.mapSize;
		tileSheet = rhs.tileSheet;

		delete[] levelData;
		uint* temp = new uint[mapSize.x * mapSize.y];
		for (uint i = 0; i < mapSize.x * mapSize.y; i++)
		{
			temp[i] = rhs.levelData[i];
		}

		levelData = temp;
	}
	return *this;
}

TileMap& TileMap::operator=(TileMap&& rhs) noexcept
{
	if (this != &rhs)
	{
		mapSize = rhs.mapSize;
		tileSheet = rhs.tileSheet;
		
		delete[] levelData;
		levelData = rhs.levelData;
		rhs.levelData = nullptr;
	}
	return *this;
}

void TileMap::Init(TileSheet* _tileSheet)
{
	tileSheet = _tileSheet;
}



void TileMap::LoadCsv(const char* file)
{
	levelData = new uint[mapSize.x * mapSize.y];
	int levelDataIndex = 0;

	char tileValue[TILEINDEXCHARACTERAMOUNT + 1]{0};	// we need 1 extra for escape character
	int tIndex = 0;

	int character = 0;
	FILE* openFile;
	openFile = fopen(file, "r");
	if (openFile)
	{
		while (character != EOF)
		{
			character = getc(openFile);

			if (character == ',' || character == '\n')
			{
				if (tIndex > TILEINDEXCHARACTERAMOUNT)
					assert(0); // "csv has values larger then tileIndexCharacterAmount allows"

				tileValue[tIndex] = '\0';
				int value = std::stoi(tileValue);
				tIndex = 0;

				
				assert(levelDataIndex < static_cast<int>(mapSize.x * mapSize.y)); // "Tilemap does not fit in the size mapSize"

				levelData[levelDataIndex++] = value;

			}
			else if (character != EOF)
			{
				tileValue[tIndex++] = static_cast<char>(character);
			}

		}
			
		fclose(openFile);
	}
	else
	{
		assert(0); // File not found
	}


}

void TileMap::DrawTileMap(Surface& target, int2 drawPosition) const
{
	uint2 spriteSize = tileSheet->GetCellSize();
	for (uint y = 0; y < mapSize.y; y++)
	{
		for (uint x = 0; x < mapSize.x; x++)
		{
			tileSheet->DrawTile(target, drawPosition + int2(x* spriteSize.x, y* spriteSize.y), levelData[y * mapSize.x + x]);
		}
	}
}

void TileMap::DrawTileMap(Surface& target, int2 drawPosition, int2 topLeft, int2 bottomRight) const
{
	uint2 spriteSize = tileSheet->GetCellSize();
	for (int y = topLeft.y; y <= bottomRight.y; y++)
	{
		for (int x = topLeft.x; x <= bottomRight.x; x++)
		{
			tileSheet->DrawTile(target, drawPosition + int2(x * spriteSize.x, y * spriteSize.y), levelData[y * mapSize.x + x]);
		}
	}
}


void TileMap::DrawTile(Surface& target, int2 drawPosition, int2 tilePosition) const
{
	uint2 spriteSize = tileSheet->GetCellSize();
	
	tileSheet->DrawTile(target, drawPosition + int2(tilePosition.x * spriteSize.x, tilePosition.y * spriteSize.y), levelData[tilePosition.y * mapSize.x + tilePosition.x]);
}

void TileMap::DrawHalfTile(Surface& target, int2 drawPosition, int2 tilePosition) const
{
	uint2 spriteSize = tileSheet->GetCellSize();

	tileSheet->DrawTopHalfTile(target, drawPosition + int2(tilePosition.x * spriteSize.x, tilePosition.y * spriteSize.y), levelData[tilePosition.y * mapSize.x + tilePosition.x]);
}





uint TileMap::GetTile(int2 tilePosition) const
{
	return levelData[tilePosition.y*mapSize.x + tilePosition.x];
}

uint TileMap::GetTile(uint tilePosition) const
{
	return levelData[tilePosition];
}

void TileMap::SetTile(int2 tilePosition, uint tile)
{
	levelData[tilePosition.y * mapSize.x + tilePosition.x] = tile;
}

void TileMap::SetTile(uint tilePosition, uint tile)
{
	levelData[tilePosition] = tile;
}

bool TileMap::Raycast(float2 start, float2 end, const ArrayList<uint>& moveableTiles, float2& hitPoint) const
{
	// Got this from my intake project 

	//// Written by Javidx9
	//// https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_RayCastDDA.cpp
	//// almost worked without changing anything only the variable names that are outside this function
	//// but some more resources I used to understand it a bit more
	//// https://gis.stackexchange.com/questions/70862/how-to-discretise-a-line-into-grid-lines
	//// https://stackoverflow.com/questions/3270840/find-the-intersection-between-line-and-grid-in-a-fast-manner
	//// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm (Does skip some squares and is not good for this situation it did help me understand how line-grid intersection algorithms are written)
	//// Additional help from Ismar, Oktay Can (Y1 PR)



	float2 endCell = float2(end.x / float(tileSheet->GetTileSize().x), end.y / float(tileSheet->GetTileSize().y));
	float2 startCell = float2(start.x / float(tileSheet->GetTileSize().x), start.y / float(tileSheet->GetTileSize().y));

	float2 rayDir = float2( end.x - start.x, end.y - start.y );
	float length = sqrt(rayDir.x * rayDir.x + rayDir.y * rayDir.y);
	if (length == 0) return false; rayDir /= length;

	float2 rayUnitStepSize = float2( abs(1.0f / rayDir.x), abs(1.0f / rayDir.y));
	int2 mapCheck = int2(int(startCell.x), int(startCell.y));
	float2 rayLength1D{};
	int2 step{};

	// Did we start stuck?
	if (moveableTiles.find(GetTile(mapCheck))==-1) 
	{
		hitPoint = start;
		return false;
	};

	if (rayDir.x < 0)
	{
		step.x = -1;
		rayLength1D.x = (startCell.x - float(mapCheck.x)) * rayUnitStepSize.x;
	}
	else
	{
		step.x = 1;
		rayLength1D.x = (float(mapCheck.x + 1) - startCell.x) * rayUnitStepSize.x;
	}

	if (rayDir.y < 0)
	{
		step.y = -1;
		rayLength1D.y = (startCell.y - float(mapCheck.y)) * rayUnitStepSize.y;
	}
	else
	{
		step.y = 1;
		rayLength1D.y = (float(mapCheck.y + 1) - startCell.y) * rayUnitStepSize.y;
	}

	float distance = 0.f;
	float maxDistance = 100.f;
	float tileLength = length / tileSheet->GetTileSize().x;

	while (distance < maxDistance)
	{
		// Walk along shortest path
		if (rayLength1D.x < rayLength1D.y)
		{
			mapCheck.x += step.x;
			distance = rayLength1D.x;
			rayLength1D.x += rayUnitStepSize.x;
		}
		else
		{
			mapCheck.y += step.y;
			distance = rayLength1D.y;
			rayLength1D.y += rayUnitStepSize.y;
		}

		if (distance > tileLength) break;

		if (moveableTiles.find(GetTile(mapCheck)) == -1)
		{
			
			hitPoint = (startCell + rayDir * distance) * static_cast<float>(tileSheet->GetTileSize().x);
			return false;
		}
	}

	return true;
}

