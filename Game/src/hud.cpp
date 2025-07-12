#include "precomp.h"

#include "resourceManager.h"
#include "tilesheet.h"

#include "aabb.h"
#include "gameobject.h"
#include "UIBase.h"
#include "dialogueManager.h"

#include "tilesheet.h"
#include "font.h"
#include "radarUI.h"
#include "inventorySlots.h"
#include "healthBar.h"

#include "hud.h"

void HUD::Init(const ResourceManager& resourceManager, Level* _level, Player* _player, EnemySystem* _enemySystem, Inventory* inventory, DialogueManager* _dialogueManager, irrklang::ISoundEngine* _soundEngine)
{
	soundEngine = _soundEngine;
	radar.Init(resourceManager, _level, _player, _enemySystem);
	healthBar.Init(resourceManager, _player);
	invenorySlots.Init(resourceManager, inventory);
	dialogueManager = _dialogueManager;
	callText = resourceManager.getTileSheet(TileSheets::CallText);
}

void HUD::Update(float deltaTime)
{
	radar.Update(deltaTime);
	healthBar.Update(deltaTime);

	if (callTextTimer.elapsed() >= 1.f && dialogueManager->GetActiveDialogue() != nullptr)
	{
		callTextTimer.reset();
		showCallText = !showCallText;
	}
	else if (dialogueManager->GetActiveDialogue() == nullptr)
	{
		showCallText = false;
	}
}

void HUD::Draw(Surface& target, int2) const
{
	// DRAW UI
	uint backColor = 0xff060606; // 0xff060606;


	target.Bar(0, 0, target.GetWidth() - 1, gameplayOffset.y - 1, backColor);
	target.Bar(0, 0, gameplayOffset.x - 1, target.GetHeight() - 1, backColor);
	target.Bar(0, gameplayOffset.y + ROOM_HEIGHT * TERRAIN_TILE_SIZE, target.GetWidth() - 1, target.GetHeight() - 1, backColor);
	target.Bar(gameplayOffset.x + ROOM_WIDTH * TERRAIN_TILE_SIZE, 0, target.GetWidth() - 1, target.GetHeight() - 1, backColor);

	// RADAR

	radar.Draw(target, int2(gameplayOffset.x + ROOM_WIDTH * TERRAIN_TILE_SIZE + 4, 24));
	invenorySlots.Draw(target, int2(gameplayOffset.x + ROOM_WIDTH * TERRAIN_TILE_SIZE + 10, 112));

	healthBar.Draw(target, int2(gameplayOffset.x + 8, gameplayOffset.y + ROOM_HEIGHT * TERRAIN_TILE_SIZE + 3));

	if (showCallText)
	{
		callText->DrawTile(target, int2(gameplayOffset.x + ROOM_WIDTH * TERRAIN_TILE_SIZE + 17, 10),0);
	}
}
