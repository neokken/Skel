#include "precomp.h"

#include "resourceManager.h"
#include "dialogueManager.h"


#include "tilesheet.h"
#include "font.h"
#include "aabb.h"
#include "gameobject.h"
#include "pathfinder.h"
#include "entity.h"
#include "objectPool.h"
#include "player.h"
#include "pickupable.h"
#include "elevator.h"

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

#include "elevatorTrigger.h"

ElevatorTrigger::ElevatorTrigger(const AABB& aabb, Level& level, Player& player, bool crawlActivatable, Elevator* elevator, MetalGear* game)
	: LevelTrigger(aabb, level, player, crawlActivatable)
	, elevator{ elevator }
	, game{ game }
{
}

void ElevatorTrigger::ActivateTrigger()
{
	if (elevator->GetElevatorState() == ElevatorState::Open)
	{
		game->SetVictory();
	}
}
