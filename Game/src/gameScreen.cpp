#include "precomp.h"

#include "inventory.h"
#include "arrayList.h"

#include "aabb.h"
#include "gameObject.h"
#include "entity.h"
#include "player.h"
#include "pickupable.h"

#include "enemySystem.h"

#include "objectPool.h"

#include "tilemap.h"
#include "level.h"

#include "tilesheet.h"
#include "font.h"

#include "UIBase.h"
#include "radarUI.h"
#include "gameScreen.h"

void GameScreen::Init(Level* _level, ObjectPool* _objectPool, Player* _player, EnemySystem* _enemySystem)
{
	level = _level;
	objectPool = _objectPool;
	player = _player;
	enemySystem = _enemySystem;
}

void GameScreen::Update(float deltaTime)
{
	player->HandleInput();

	Pickupable* item = objectPool->GetFirstPickupableCollider(player->GetActiveCollider());
	if (item)
	{
		//invetory.handlePickup(item->PickUp());
		player->GetInventory()->HandlePickupable(item->PickUp());
	}

	objectPool->Update(deltaTime);
	enemySystem->Update(deltaTime);
	
	
	player->ResetCrawlDisallow();
	level->Update(*player);
}


void GameScreen::Draw(Surface& target, int2 offset) const
{

	level->DrawVisual(target, offset);

#ifdef TERRAIN_DEBUG
	level->DrawDebug(target, offset);
#endif // TERRAIN_DEBUG


#ifdef TRIGGER_DEBUG
	level->DrawTriggers(target, offset);
#endif // TRIGGER_DEBUG


	objectPool->Draw(target, offset);



#ifdef OBJECT_DEBUG
	objectPool->DrawDebug(target, offset);
#endif // OBJECT_DEBUG
}
