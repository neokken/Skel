#include "precomp.h"


#include "aabb.h"
#include "levelTrigger.h"


#include "gameobject.h"
#include "entity.h"
#include "player.h"

#include "arrayList.h"
#include "pathfinder.h"
#include "textBox.h"
#include "enemy.h"

#include "tilemap.h"
#include "arrayList.h"

#include "level.h"


#include "roomTeleportTrigger.h"



RoomTeleportTrigger::RoomTeleportTrigger(const AABB& aabb, Level& level, Player& player, bool _crawlActivatable, float2 _teleportPosition, bool _forceUncrawl)
	: LevelTrigger(aabb, level, player, _crawlActivatable)
	, teleportPosition{ _teleportPosition }
	, forceUncrawl{ _forceUncrawl }
{
	roomPosition.x = static_cast<int>(teleportPosition.x) / TERRAIN_TILE_SIZE / ROOM_WIDTH;
	roomPosition.y = static_cast<int>(teleportPosition.y) / TERRAIN_TILE_SIZE / ROOM_HEIGHT;
}

void RoomTeleportTrigger::ActivateTrigger()
{
	
	level.SetActiveRoom(roomPosition);
	player.SetPosition(teleportPosition);
	if (forceUncrawl)
	{
		player.SetCrawl(false);
	}
	
}
