#include "precomp.h"

#include "resourceManager.h"
#include "dialogueManager.h"

#include "arrayList.h"

#include "aabb.h"
#include "gameobject.h"
#include "pathfinder.h"
#include "entity.h"
#include "objectPool.h"
#include "player.h"


#include "textBox.h"
#include "enemy.h"
#include "enemySystem.h"

#include "tilemap.h"

#include "level.h"

#include "UIBase.h"
#include "radarUI.h"
#include "healthBar.h"
#include "inventorySlots.h"
#include "hud.h"

#include "gameScreen.h"
#include "textBox.h"
#include "InventoryScreen.h"

#include "animatedTextBox.h"
#include "codecTextBox.h"
#include "codec.h"
#include "portraitFrame.h"
#include "codecScreen.h"
#include "saveLoadScreen.h"
#include "screenManager.h"

#include "inventory.h"
#include "metalGear.h"

#include "levelTeleportTrigger.h"

LevelTeleportTrigger::LevelTeleportTrigger(const AABB& aabb, Level& level, Player& player, bool crawlActivatable, MetalGear* game, int levelIndex, float2 teleportPos, bool forceUnCrawl)
	: LevelTrigger(aabb, level, player, crawlActivatable)
	, teleportPosition{ teleportPos }
	, levelIndex{ levelIndex }
	, game{ game }
	, forceUnCrawl{ forceUnCrawl }
{
	roomPosition.x = static_cast<int>(teleportPosition.x) / TERRAIN_TILE_SIZE / ROOM_WIDTH;
	roomPosition.y = static_cast<int>(teleportPosition.y) / TERRAIN_TILE_SIZE / ROOM_HEIGHT;
}

void LevelTeleportTrigger::ActivateTrigger()
{
	game->LoadLevel(levelIndex, roomPosition, teleportPosition);
}