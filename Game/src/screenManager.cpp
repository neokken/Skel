#include "precomp.h"
#include "resourceManager.h"

#include "tilesheet.h"

#include "aabb.h"
#include "gameobject.h"

#include "UIBase.h"
#include "textBox.h"
#include "animatedTextBox.h"
#include "codecTextBox.h"

#include "portraitFrame.h"
#include "codec.h"
#include "codecScreen.h"
#include "gameScreen.h"
#include "InventoryScreen.h"
#include "saveLoadScreen.h"

#include "screenManager.h"

#include "Core/Engine.h"


void ScreenManager::Init(const ResourceManager& resourceManager, Level* _level, ObjectPool* _objectpool, Player* _player, EnemySystem* _enemySystem, Inventory* _inventory, DialogueManager* dialogueManager, MetalGear* game, irrklang::ISoundEngine* _soundEngine)
{
	inputManager = &Engine::GetInstance().GetInput();
	soundEngine = _soundEngine;
	pauseText = resourceManager.getTileSheet(TileSheets::PauseText);
	gameplayScreen.Init(_level, _objectpool, _player, _enemySystem);

	weaponScreen.Init(resourceManager, _inventory, true);
	equipmentScreen.Init(resourceManager, _inventory, false);
	codecScreen.Init(resourceManager, dialogueManager);
	saveLoadScreen.Init(resourceManager, game);
}

void ScreenManager::Update(float deltaTime)
{
	if (inputManager->IsKeyJustDown(GLFW_KEY_F1))
	{
		if (screenState != ScreenState::GameplayPaused)
		{
			screenState = ScreenState::GameplayPaused;
		}
		else if (screenState == ScreenState::GameplayPaused)
		{
			screenState = ScreenState::Gameplay;
		}

	}

	if (inputManager->IsKeyJustDown(GLFW_KEY_F2))
	{
		if (screenState != ScreenState::Weapons)
		{
			screenState = ScreenState::Weapons;
		}
		else
		{
			screenState = ScreenState::Gameplay;
		}

	}

	if (inputManager->IsKeyJustDown(GLFW_KEY_F3))
	{
		if (screenState != ScreenState::Equipment)
		{
			screenState = ScreenState::Equipment;
		}
		else
		{
			screenState = ScreenState::Gameplay;
		}

	}

	if (inputManager->IsKeyJustDown(GLFW_KEY_F4))
	{
		if (screenState != ScreenState::Codec)
		{
			screenState = ScreenState::Codec;
			codecScreen.OpenCodec();
		}
		else
		{
			screenState = ScreenState::Gameplay;
		}

	}

	if (inputManager->IsKeyJustDown(GLFW_KEY_F5))
	{
		if (screenState != ScreenState::SaveLoad)
		{
			screenState = ScreenState::SaveLoad;
			codecScreen.OpenCodec();
		}
		else
		{
			screenState = ScreenState::Gameplay;
		}

	}

	switch (screenState)
	{
	case ScreenState::Gameplay:
		gameplayScreen.Update(deltaTime);
		break;
	case ScreenState::GameplayPaused:
		// nothing gets updated here
		break;
	case ScreenState::Codec:
		codecScreen.Update(deltaTime);
		// will call radio.Update() when radio is implemented
		break;
	case ScreenState::Weapons:
		weaponScreen.Update(deltaTime);
		break;
	case ScreenState::Equipment:
		equipmentScreen.Update(deltaTime);
		break;
	case ScreenState::SaveLoad:
		saveLoadScreen.Update(deltaTime);
		break;
	}
}

void ScreenManager::Draw(Surface& target, int2 offset) const
{
	int2 position = int2((ROOM_WIDTH * TERRAIN_TILE_SIZE - 38) / 2, (ROOM_HEIGHT * TERRAIN_TILE_SIZE - 15) / 2);

	switch (screenState)
	{
	
	case ScreenState::GameplayPaused:		
		gameplayScreen.Draw(target, offset);

		pauseText->DrawTile(target, position + offset, 0);
		break;
	case ScreenState::Gameplay:
		gameplayScreen.Draw(target, offset);
		break;
	case ScreenState::Codec:
		codecScreen.Draw(target, offset);
		break;
	case ScreenState::Weapons:
		weaponScreen.Draw(target, offset);
		break;
	case ScreenState::Equipment:
		equipmentScreen.Draw(target, offset);
		break;
	case ScreenState::SaveLoad:
		saveLoadScreen.Draw(target,offset);
		break;
	default:
		break;
	}
}

void ScreenManager::SetScreen(ScreenState value)
{
	screenState = value;
}

ScreenState ScreenManager::GetScreenState() const
{
	return screenState;
}
