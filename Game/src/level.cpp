#include "precomp.h"

#include "aabb.h"
#include "gameobject.h"
#include "entity.h"
#include "player.h"
#include "dialogueManager.h"

#include "tilemap.h"

#include "levelTrigger.h"
#include "roomTeleportTrigger.h"
#include "dialogueTrigger.h"
#include "levelTeleportTrigger.h"
#include "crawlTrigger.h"
#include "elevatorTrigger.h"

#include "arrayList.h"

#include "level.h"

Level::Level(const char* levelVisualMapPath, const char* levelCollisionMapPath, uint levelWidth, uint levelHeight)
	: levelSize{uint2(levelWidth,levelHeight)}
	, visualLayer{levelVisualMapPath, levelSize}
	, collisionLayer{levelCollisionMapPath, levelSize}
{

}


Level::Level(const Level& other)
{
	levelSize = other.levelSize;
	activeRoom = other.activeRoom;
	worldOffset = other.worldOffset;

	visualLayer = other.visualLayer;
	collisionLayer = other.collisionLayer;

	for (uint i = 0; i < other.levelTriggerCount; i++)
	{
		levelTriggers[i] = other.levelTriggers[i];
	}
}

Level::Level(Level&& other) noexcept
{
	levelSize = other.levelSize;
	activeRoom = other.activeRoom;
	worldOffset = other.worldOffset;

	visualLayer = other.visualLayer;
	collisionLayer = other.collisionLayer;

	for (uint i = 0; i < other.levelTriggerCount; i++)
	{
		levelTriggers[i] = other.levelTriggers[i];
		other.levelTriggers[i] = nullptr;
	}

	levelTriggerCount = other.levelTriggerCount;

	dialogueManager = other.dialogueManager;
}



Level& Level::operator=(Level&& other) noexcept
{
	if (this == &other) return *this;

	for (uint i = 0; i < levelTriggerCount; i++)
	{
		delete levelTriggers[i];
		levelTriggers[i] = nullptr;
	}

	levelSize = other.levelSize;
	activeRoom = other.activeRoom;
	worldOffset = other.worldOffset;

	visualLayer = other.visualLayer;
	collisionLayer = other.collisionLayer;

	for (uint i = 0; i < other.levelTriggerCount; i++)
	{
		levelTriggers[i] = other.levelTriggers[i];
		other.levelTriggers[i] = nullptr;
	}

	levelTriggerCount = other.levelTriggerCount;
	dialogueManager = other.dialogueManager;
	return *this;
}



Level::~Level()
{
	for (uint i = 0; i < MAX_TRIGGERS_AMOUNT; i++)
	{
		delete levelTriggers[i];
		levelTriggers[i] = nullptr;
	}
	
}

void Level::Init(TileSheet* visual, TileSheet* collision, DialogueManager* _dialogueManager)
{
	visualLayer.Init(visual);
	collisionLayer.Init(collision);

	walkableColliders.reserve(4);
	walkableColliders.append(CollisionTileType::WALKABLE_OPEN);
	walkableColliders.append(CollisionTileType::FORCED_CRAWL_OVERLAY);
	walkableColliders.append(CollisionTileType::WALKABLE_CLOSED);
	walkableColliders.append(CollisionTileType::CRAWL_COLLISION);

	dialogueManager = _dialogueManager;
}

void Level::Update(Player& player)
{
	int2 temp = worldToScreenSpace(player.GetPosition());
	float2 playerPosLocal = float2{static_cast<float>(temp.x),static_cast<float>(temp.y) };

	int2 currentRoom = GetActiveRoom();

	uint2 playerSize = tileSheetData[static_cast<int>(TileSheets::Snake)].cellSize;
	if (playerPosLocal.x < -static_cast<float>(playerSize.x) / 2.f)
	{
		currentRoom.x--;
		playerPosLocal = float2(ROOM_WIDTH * TERRAIN_TILE_SIZE - static_cast<float>(playerSize.x) / 2.f, playerPosLocal.y);
	}
	else if (playerPosLocal.x > static_cast<float>(ROOM_WIDTH * TERRAIN_TILE_SIZE) - static_cast<float>(playerSize.x) / 2.f)
	{
		currentRoom.x++;
		playerPosLocal = float2(-static_cast<float>(playerSize.x) / 2.f, playerPosLocal.y);
	}
	else if (playerPosLocal.y < -static_cast<float>(playerSize.y) / 4.f * 3.f)
	{
		currentRoom.y--;
		playerPosLocal = float2(playerPosLocal.x, ROOM_HEIGHT * TERRAIN_TILE_SIZE - static_cast<float>(playerSize.y) / 5.f * 4.f);
	}
	else if (playerPosLocal.y > static_cast<float>(ROOM_HEIGHT * TERRAIN_TILE_SIZE) - static_cast<float>(playerSize.y) / 5.f * 4.f)
	{
		currentRoom.y++;
		playerPosLocal = float2(playerPosLocal.x, -static_cast<float>(playerSize.y) / 2.f);
	}

	if ((activeRoom.x!= currentRoom.x || activeRoom.y != currentRoom.y) && 
		currentRoom.x >= 0 && currentRoom.x < static_cast<int>(levelSize.x / ROOM_WIDTH) && 
		currentRoom.y >= 0 && currentRoom.x < static_cast<int>(levelSize.y / ROOM_HEIGHT))
	{
		SetActiveRoom(currentRoom);
		player.SetPosition(screenToWorldSpace(playerPosLocal));
	}


	dialogueManager->ClearActiveDialogue();
	for (uint i = 0; i < levelTriggerCount; i++)
	{
		if (levelTriggers[i]->CheckCollision(player.GetActiveCollider()))
		{
			levelTriggers[i]->ActivateTrigger();
		}
	}

}


void Level::DrawVisual(Surface& target, int2 offset)
{
	int2 start = int2(-worldOffset.x / TERRAIN_TILE_SIZE, -worldOffset.y / TERRAIN_TILE_SIZE);
	visualLayer.DrawTileMap(target, worldOffset + offset, start, start + int2(ROOM_WIDTH-1,ROOM_WIDTH-1));
}

void Level::DrawTile(Surface& target, int2 tilePositon, int2 offset)
{
	visualLayer.DrawTile(target, worldOffset + offset, tilePositon);
}

void Level::DrawTopHalfTile(Surface& target, int2 tilePositon, int2 offset)
{
	visualLayer.DrawHalfTile(target, worldOffset + offset, tilePositon);
}

void Level::DrawDebug(Surface& target, int2 offset)
{
	int2 start = int2(-worldOffset.x / TERRAIN_TILE_SIZE, -worldOffset.y / TERRAIN_TILE_SIZE);
	collisionLayer.DrawTileMap(target, worldOffset + offset, start, start + int2(ROOM_WIDTH - 1, ROOM_WIDTH - 1));
}

void Level::DrawTriggers(Surface& target, int2 offset)
{
	for (uint i = 0; i < levelTriggerCount; i++)
	{
		levelTriggers[i]->DrawDebug(target, offset, 0xffffffff);
	}
}

void Level::ResolvePlayerCollision(Player& player) const
{
	AABB playerAABB = player.GetActiveCollider();

	int2 topLeftTile = int2(static_cast<int>(playerAABB.topLeft.x) / static_cast<int>(TERRAIN_TILE_SIZE), 
							static_cast<int>(playerAABB.topLeft.y) / static_cast<int>(TERRAIN_TILE_SIZE));

	int2 bottomRightTile = int2(static_cast<int>(playerAABB.bottomRight.x) / static_cast<int>(TERRAIN_TILE_SIZE),
								static_cast<int>(playerAABB.bottomRight.y) / static_cast<int>(TERRAIN_TILE_SIZE));
	// clamping

	topLeftTile.x = std::min(std::max(topLeftTile.x, 0), static_cast<int>(levelSize.x - 1));
	topLeftTile.y = std::min(std::max(topLeftTile.y, 0), static_cast<int>(levelSize.y - 1));

	bottomRightTile.x = std::min(std::max(bottomRightTile.x, 0), static_cast<int>(levelSize.x - 1));
	bottomRightTile.y = std::min(std::max(bottomRightTile.y, 0), static_cast<int>(levelSize.y - 1));


	for (int y = topLeftTile.y; y <= bottomRightTile.y; y++)
	{
		for (int x = topLeftTile.x; x <= bottomRightTile.x; x++)
		{

			if (!IsWalkable(GetColliderTile(int2(x, y))) || GetColliderTile(int2(x, y)) == CollisionTileType::CRAWL_COLLISION)
			{
				float2 tilePosition = float2(static_cast<float>(x * TERRAIN_TILE_SIZE), static_cast<float>(y * TERRAIN_TILE_SIZE));
				AABB tileAABB = AABB(tilePosition, tilePosition + float2(TERRAIN_TILE_SIZE - 1));

				player.CheckCollision(tileAABB, GetColliderTile(int2(x, y)));
			}
		}
	}
}


bool Level::RaycastCollision(const float2& point1, const float2& point2, float2& hitPoint) const
{

	ArrayList<uint> walkable;
	for (uint i = 0; i < walkableColliders.size(); i++)
	{
		walkable.append(static_cast<uint>(walkableColliders.at(i)));
	}
	

	return collisionLayer.Raycast(point1, point2, walkable, hitPoint);
}

int2 Level::worldToScreenSpace(int2 position) const
{
	return position + worldOffset;
}
int2 Level::worldToScreenSpace(float2 position) const
{
	return worldToScreenSpace(int2(static_cast<int>(position.x), static_cast<int>(position.y)));
}

float2 Level::screenToWorldSpace(int2 position) const
{
	return float2(static_cast<float>(position.x - worldOffset.x), static_cast<float>(position.y - worldOffset.y));
}

float2 Level::screenToWorldSpace(float2 position) const
{
	return position - float2(static_cast<float>(worldOffset.x), static_cast<float>(worldOffset.y));
}

bool Level::IsWalkable(CollisionTileType collider) const
{
	return walkableColliders.find(collider) == -1 ? false : true;
}




const uint2& Level::GetLevelSize() const
{
	return levelSize;
}

const int2& Level::GetActiveRoom() const
{
	return activeRoom;
}

const int2& Level::GetWorldOffset() const
{
	return worldOffset;
}

CollisionTileType Level::GetColliderTile(int2 tilePosition) const
{
	if (tilePosition.x < 0 || tilePosition.x >= static_cast<int>(GetLevelSize().x) ||
		tilePosition.y < 0 || tilePosition.y >= static_cast<int>(GetLevelSize().y))
	{
		return CollisionTileType::UNKNOWN;
	}

	uint tile = collisionLayer.GetTile(tilePosition);
	return static_cast<CollisionTileType>(tile);
}

void Level::SetActiveRoom(const int2& room)
{
	if (room.x < 0 || room.x >= static_cast<int>(levelSize.x/ROOM_WIDTH) || room.y < 0 || room.y >= static_cast<int>(levelSize.y/ROOM_HEIGHT))
		return;

	activeRoom = room;
	worldOffset = -int2(room.x * ROOM_WIDTH * TERRAIN_TILE_SIZE, room.y * ROOM_HEIGHT * TERRAIN_TILE_SIZE);
}

void Level::AddRoomTeleportTrigger(const AABB& collider, Player& player, float2 playerPosition, bool crawlActivatable, bool forceUncrawl)
{
	assert(levelTriggerCount < MAX_TRIGGERS_AMOUNT); // buffer already full
	if (levelTriggerCount < MAX_TRIGGERS_AMOUNT)
	{
		levelTriggers[levelTriggerCount] = new RoomTeleportTrigger(collider, *this, player, crawlActivatable, playerPosition, forceUncrawl);
		levelTriggerCount++;
	}
}

void Level::AddLevelTeleportTrigger(const AABB& collider, Player& player, MetalGear* game, int levelIndex, float2 playerPosition, bool forceUnCrawl, bool crawlActivatable)
{
	assert(levelTriggerCount < MAX_TRIGGERS_AMOUNT); // buffer already full
	if (levelTriggerCount < MAX_TRIGGERS_AMOUNT)
	{
		levelTriggers[levelTriggerCount] = new LevelTeleportTrigger(collider, *this, player, crawlActivatable, game, levelIndex, playerPosition, forceUnCrawl);
		levelTriggerCount++;
	}
}

void Level::AddDialogueTrigger(const AABB& collider, Player& player, const CodecDialogue* dialogue, bool crawlActivatable)
{
	assert(levelTriggerCount < MAX_TRIGGERS_AMOUNT); // buffer already full
	if (levelTriggerCount < MAX_TRIGGERS_AMOUNT)
	{
		levelTriggers[levelTriggerCount] = new DialogueTrigger(collider, *this, player, crawlActivatable, dialogueManager, dialogue);
		levelTriggerCount++;
	}
		
}

void Level::AddCrawlTrigger(const AABB& collider, Player& player, bool vertical, float minRange, float maxRange)
{
	assert(levelTriggerCount < MAX_TRIGGERS_AMOUNT); // buffer already full
	if (levelTriggerCount < MAX_TRIGGERS_AMOUNT)
	{
		levelTriggers[levelTriggerCount] = new CrawlTrigger(collider, *this, player, true, vertical, minRange, maxRange);
		levelTriggerCount++;
	}
}

void Level::AddElevatorTrigger(const AABB& collider, Player& player, Elevator* elevator, MetalGear* game, bool crawlActivatable)
{
	assert(levelTriggerCount < MAX_TRIGGERS_AMOUNT); // buffer already full
	if (levelTriggerCount < MAX_TRIGGERS_AMOUNT)
	{
		levelTriggers[levelTriggerCount] = new ElevatorTrigger(collider, *this, player, crawlActivatable,elevator,game);
		levelTriggerCount++;
	}
}

