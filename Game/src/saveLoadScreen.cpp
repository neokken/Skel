#include "precomp.h"

#include "textBox.h"

#include "resourceManager.h"
#include "dialogueManager.h"

#include "aabb.h"
#include "gameobject.h"
#include "pathfinder.h"
#include "entity.h"
#include "objectPool.h"
#include "player.h"
#include "pickupable.h"

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

#include "saveLoadScreen.h"

#include "Core/Engine.h"

void SaveLoadScreen::Init(const ResourceManager& resourceManager, MetalGear* _game)
{
	game = _game;
	inputManager = &Engine::GetInstance().GetInput();
	textbox.Init(resourceManager);
	textbox.SetSize(int2(57, 28));
}

void SaveLoadScreen::Update(float)
{
	if (inputManager->IsKeyJustDown(GLFW_KEY_ENTER))
	{
		if (currentOption == 0)
		{
			printf("GameState saved to file.\n");
			game->WriteGameData(game->GetGameData());
		}
		else
		{
			printf("GameState loaded from save.\n");
			game->ReadGameData();
		}
	}

	if (inputManager->IsKeyJustDown(GLFW_KEY_DOWN))
	{
		currentOption++;
		currentOption %= optionAmount;
	}

	if (inputManager->IsKeyJustDown(GLFW_KEY_UP))
	{
		currentOption--;
		currentOption = (currentOption+ optionAmount)%optionAmount;
	}

	if (currentOption == 0)
	{
		textbox.SetText("[Save]\n Load");
	}
	else
	{
		textbox.SetText(" Save\n[Load]");
	}

}

void SaveLoadScreen::Draw(Surface& target, int2 offset) const
{
	target.Bar(offset.x, offset.y, offset.x + ROOM_WIDTH * TERRAIN_TILE_SIZE, offset.y + ROOM_HEIGHT * TERRAIN_TILE_SIZE, 0xff060606);
	textbox.Draw(target, offset);
}
