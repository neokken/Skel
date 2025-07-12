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

#include "musicTrack.h"
#include "inventory.h"


#include "metalGear.h"

#include "Core/Engine.h"

MetalGear::MetalGear()
{
	level = Level(OUTSIDE_LEVEL_VISUAL_PATH, OUTSIDE_LEVEL_COLLISION_PATH, 24 * 3, 24 * 5);
	inventory = new Inventory();
	music = new MusicTrack();
	codecCall = new MusicTrack();
}

MetalGear::~MetalGear()
{
	delete music;
	delete codecCall;
}

void MetalGear::Init(ResourceManager* _resourceManager, irrklang::ISoundEngine* _soundEngine)
{
	resourceManager = _resourceManager;
	soundEngine = _soundEngine;
	music->Init(soundEngine);
	codecCall->Init(soundEngine);
	objectPool.Init(&level, &enemySystem, resourceManager, _soundEngine);
	textbox.Init(*resourceManager);

	mainMenuTexture = resourceManager->getTileSheet(TileSheets::MainMenuTexture);

	player = objectPool.AddPlayer(float2(0.f), inventory);

	enemySystem.Init(&objectPool, &level, player, soundEngine, music);


	gameHUD.Init(*resourceManager, &level, player, &enemySystem, inventory, &dialogueManager, soundEngine);
	gameScreens.Init(*resourceManager, &level, &objectPool, player, &enemySystem, inventory, &dialogueManager, this, soundEngine);

	LoadLevel(0);
	

	music->PlayTrack("assets/Sounds/mainMenu.ogg");
	codecCall->PlayTrack("assets/Sounds/codec.ogg");
	codecCall->Pause(true);

}

void MetalGear::Update(float deltaTime)
{

	switch (menuState)
	{
	case MenuState::Game:
		if (!player->IsDead())
		{
			gameScreens.Update(deltaTime);
			gameHUD.Update(deltaTime);
			codecCall->Pause(!dialogueManager.GetActiveDialogue());

		}
		else
		{
			player->Update(deltaTime);
			music->Pause(true);
		}
		if (player->DeathCompleted())
		{
			music->Pause(false);
			SetDead();
		}
		break;
	case MenuState::MainMenu:
		UpdateMainMenu(deltaTime);

		break;
	case MenuState::DeathMenu:
		UpdateDeathMenu(deltaTime);
		break;
	case MenuState::VictoryMenu:
		UpdateVictoryMenu(deltaTime);
		break;
	default:
		break;
	}


}

void MetalGear::Draw(Surface& target) const
{
	switch (menuState)
	{
	case MenuState::Game:
		gameScreens.Draw(target, int2(8));
		gameHUD.Draw(target);
		break;
	case MenuState::MainMenu:
		DrawMainMenu(target);

		break;
	case MenuState::DeathMenu:
		DrawDeathMenu(target);
		break;
	case MenuState::VictoryMenu:
		DrawVictoryMenu(target);
		break;
	default:
		break;
	}
}

void MetalGear::LoadLevel(int _level)
{
	// clear objectpool
	// * remove everything except player

	objectPool.ClearForLevel();
	enemySystem.SetAlarmState(AlarmState::Normal);

	switch (_level)
	{
	case 0:
		LoadLevel0();
		break;
	case 1:
		LoadLevel1();
		break;
	case 2:
		break;
	default:
		break;
	}
}

void MetalGear::LoadLevel(int levelIndex, int2 roomPosition, float2 teleportPosition)
{
	LoadLevel(levelIndex);
	level.SetActiveRoom(roomPosition);
	player->SetPosition(teleportPosition);

}

Player* MetalGear::GetPlayer() const
{
	return player;
}

void MetalGear::LoadGameData(const GameData& gameData)
{

	music->PlayTrack("assets/Sounds/normalMusic.ogg");
	enemySystem.SetAlarmState(AlarmState::Normal);
	LoadLevel(gameData.currentLevel);
	level.SetActiveRoom(gameData.currentRoom);

	player->Reset();
	player->SetCurrentHealth(gameData.currentHealth);
	player->SetMaxHealth(gameData.maxHealth);
	player->SetLookDirection(gameData.lookdir);

	player->SetPosition(float2(static_cast<float>(gameData.playerPosition.x), static_cast<float>(gameData.playerPosition.y)));
	player->SetCrawl(gameData.isCrawling);


	inventory->ClearItemLists();
	for (int i = 0; i < INVENTORY_SLOT_COUNT; i++)
	{
		inventory->HandlePickupable(gameData.equipment[i]);
		inventory->HandlePickupable(gameData.weapons[i]);
	}

	inventory->SetAmmoCount(gameData.ammoCount);


	for (int i = 0; i < objectPool.GetSize(); i++)
	{
		GameObject* object = objectPool.GetObjectAt(i);

		Pickupable* p = dynamic_cast<Pickupable*>(object);
		if (p != nullptr)
		{
			int2 position = { static_cast<int>(p->GetPosition().x),static_cast<int>(p->GetPosition().y) };
			bool found = false;

			for (int j = 0; j < 8; j++)
			{
				if (position.x == gameData.pickupableLocations[j].x && position.y == gameData.pickupableLocations[j].y)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				p->MarkForDelete();
			}
		}
	}

	objectPool.Update(0.f);
}

GameData MetalGear::GetGameData() const
{
	GameData newData;
	
	newData.playerPosition = { static_cast<int>(player->GetPosition().x),static_cast<int>(player->GetPosition().y)};
	newData.isCrawling = player->IsCrawling();
	newData.lookdir = player->GetLookDirection();

	newData.maxHealth = player->GetMaxHealth();
	newData.currentHealth = player->GetCurrentHealth();

	newData.currentLevel = currentLevel;
	newData.currentRoom = level.GetActiveRoom();

	newData.ammoCount = static_cast<int>(inventory->GetAmmoCount());

	const ItemType* equipm = inventory->GetEquipmentList();
	const ItemType* weapm = inventory->GetWeaponList();
	for (int i = 0; i < INVENTORY_SLOT_COUNT; i++)
	{
		newData.equipment[i] = equipm[i];
		newData.weapons[i] = weapm[i];
	}
	
	for (int i = 0; i < 8; i++)
	{
		newData.pickupableLocations[i] = int2(0);
	}

	int indx = 0;
	for (int i = 0;  i < objectPool.GetSize(); i++)
	{
		GameObject* object = objectPool.GetObjectAt(i);

		Pickupable* p = dynamic_cast<Pickupable*>(object);
		if (p != nullptr)
		{
			newData.pickupableLocations[indx] = { static_cast<int>(p->GetPosition().x),static_cast<int>(p->GetPosition().y) };
			indx++;
		}
	}

	return newData;
}

void MetalGear::ReadGameData()
{
	GameData gameData;

	FILE* file = fopen("saveData.txt", "r");

	if (file == nullptr)
	{
		printf("Could not read save file\n");
		return;
	}
	
	fread(&gameData.playerPosition, sizeof(gameData.playerPosition), 1, file);
	fread(&gameData.isCrawling, sizeof(gameData.isCrawling), 1, file);
	fread(&gameData.lookdir, sizeof(gameData.lookdir), 1, file);
	fread(&gameData.maxHealth, sizeof(gameData.maxHealth), 1, file);
	fread(&gameData.currentHealth, sizeof(gameData.currentHealth), 1, file);

	fread(&gameData.currentLevel, sizeof(gameData.currentLevel), 1, file);
	fread(&gameData.currentRoom, sizeof(gameData.currentRoom), 1, file);

	fread(&gameData.ammoCount, sizeof(gameData.ammoCount), 1, file);
	fread(&gameData.equipment, sizeof(gameData.equipment), 1, file);
	fread(&gameData.weapons, sizeof(gameData.equipment), 1, file);
	fread(&gameData.pickupableLocations, sizeof(gameData.pickupableLocations), 1, file);

	fclose(file);
	
	LoadGameData(gameData);
}

void MetalGear::WriteGameData(const GameData& gameData)
{

	FILE* file = fopen("saveData.txt", "w");

	if (file == nullptr)
	{
		printf("Could not create file\n");
		return;
	}


	fwrite(&gameData.playerPosition, sizeof(gameData.playerPosition), 1, file);

	fwrite(&gameData.isCrawling, sizeof(gameData.isCrawling), 1, file);
	fwrite(&gameData.lookdir, sizeof(gameData.lookdir), 1, file);

	fwrite(&gameData.maxHealth, sizeof(gameData.maxHealth), 1, file);
	fwrite(&gameData.currentHealth, sizeof(gameData.currentHealth), 1, file);

	fwrite(&gameData.currentLevel, sizeof(gameData.currentLevel), 1, file);
	fwrite(&gameData.currentRoom, sizeof(gameData.currentRoom), 1, file);


	fwrite(&gameData.ammoCount, sizeof(gameData.ammoCount), 1, file);

	fwrite(&gameData.equipment, sizeof(gameData.equipment), 1, file);		

	fwrite(&gameData.weapons, sizeof(gameData.equipment), 1, file);


	fwrite(&gameData.pickupableLocations, sizeof(gameData.pickupableLocations), 1, file);

	fclose(file);

}

void MetalGear::SetDead()
{
	menuState = MenuState::DeathMenu;

	music->PlayTrack("assets/Sounds/DeathMusic.ogg");
	menuOption = 0;
}

void MetalGear::SetVictory()
{
	menuState = MenuState::VictoryMenu;

	music->PlayTrack("assets/Sounds/mainMenu.ogg");
	menuOption = 0;
}

bool MetalGear::IsRunning() const
{
	return !exitGame;
}

void MetalGear::LoadLevel0()
{
	currentLevel = 0;
	level = Level(OUTSIDE_LEVEL_VISUAL_PATH, OUTSIDE_LEVEL_COLLISION_PATH, 24 * 3, 24 * 5);
	level.Init(resourceManager->getTileSheet(TileSheets::OutsideTerrain), resourceManager->getTileSheet(TileSheets::TerrainCollision), &dialogueManager);

	level.SetActiveRoom(int2(1, 4));
	player->SetPosition(float2(34.f * static_cast<float>(TERRAIN_TILE_SIZE), 107.f * static_cast<float>(TERRAIN_TILE_SIZE)));



	
	PatrolAction patrolA[]{
		{PatrolActionType::MoveTo, int2(13,78)},
		{PatrolActionType::LookRight, 5.f},
		{PatrolActionType::MoveTo, int2(5,78)},
		{PatrolActionType::LookLeft, 5.f},
		{PatrolActionType::MoveTo, int2(5,87)},
		{PatrolActionType::LookDown, 5.f},
		{PatrolActionType::MoveTo, int2(37,87)},
		{PatrolActionType::LookRight, 5.f},
		{PatrolActionType::MoveTo, int2(37,92)},
		{PatrolActionType::LookDown, 5.f},
		{PatrolActionType::MoveTo, int2(18,92)},
		{PatrolActionType::MoveTo, int2(18,69)},
		{PatrolActionType::MoveTo, int2(13,69)},
	};

	PatrolAction patrolB[]{
		{PatrolActionType::MoveTo, int2(62,89)},
		{PatrolActionType::LookRight, 5.f},
		{PatrolActionType::MoveTo, int2(62,69)},
		{PatrolActionType::MoveTo, int2(53,69)},
		{PatrolActionType::LookRight, 5.f},
		{PatrolActionType::MoveTo, int2(53,93)},
		{PatrolActionType::MoveTo, int2(38,93)},
		{PatrolActionType::LookUp, 5.f},
		{PatrolActionType::MoveTo, int2(26,93)},
		{PatrolActionType::LookLeft, 5.f},
		{PatrolActionType::MoveTo, int2(26,86)},
		{PatrolActionType::MoveTo, int2(38,86)},
		{PatrolActionType::LookDown, 5.f},
		{PatrolActionType::MoveTo, int2(53,86)},
		{PatrolActionType::MoveTo, int2(53,89)},
	};

	PatrolAction patrolC[]{
		{PatrolActionType::MoveTo, int2(65,53)},
		{PatrolActionType::MoveTo, int2(65,61)},
		{PatrolActionType::MoveTo, int2(50,61)},
		{PatrolActionType::MoveTo, int2(50,78)},
		{PatrolActionType::MoveTo, int2(22,78)},
		{PatrolActionType::MoveTo, int2(22,62)},
		{PatrolActionType::MoveTo, int2(9,62)},
		{PatrolActionType::LookRight, 5.f},
		{PatrolActionType::MoveTo, int2(9,69)},
		{PatrolActionType::MoveTo, int2(5,69)},
		{PatrolActionType::LookUp, 5.f},
		{PatrolActionType::MoveTo, int2(2,69)},
		{PatrolActionType::MoveTo, int2(2,58)},
		{PatrolActionType::MoveTo, int2(2,69)},
		{PatrolActionType::MoveTo, int2(9,69)},
		{PatrolActionType::MoveTo, int2(22,62)},
		{PatrolActionType::MoveTo, int2(22,78)},
		{PatrolActionType::MoveTo, int2(50,78)},
		{PatrolActionType::MoveTo, int2(50,61)},
		{PatrolActionType::MoveTo, int2(65,61)},
	};

	PatrolAction patrolD[]{
		{PatrolActionType::MoveTo, int2(61,53)},
		{PatrolActionType::LookDown, 5.f},
		{PatrolActionType::MoveTo, int2(45,53)},
		{PatrolActionType::MoveTo, int2(45,69)},
		{PatrolActionType::MoveTo, int2(38,69)},
		{PatrolActionType::LookUp, 5.f},
		{PatrolActionType::MoveTo, int2(26,69)},
		{PatrolActionType::MoveTo, int2(26,54)},
		{PatrolActionType::MoveTo, int2(6,54)},
		{PatrolActionType::MoveTo, int2(6,58)},
		{PatrolActionType::LookDown, 5.f},
		{PatrolActionType::MoveTo, int2(6,54)},
		{PatrolActionType::MoveTo, int2(26,54)},
		{PatrolActionType::MoveTo, int2(26,69)},
		{PatrolActionType::MoveTo, int2(45,69)},
		{PatrolActionType::MoveTo, int2(45,53)},
	};

	PatrolAction patrolE[]{
		{PatrolActionType::MoveTo, int2(2,44)},
		{PatrolActionType::LookDown, 5.f},
		{PatrolActionType::MoveTo, int2(2,35)},
		{PatrolActionType::MoveTo, int2(7,35)},
		{PatrolActionType::MoveTo, int2(7,41)},
		{PatrolActionType::LookDown, 5.f},
		{PatrolActionType::MoveTo, int2(10,41)},
		{PatrolActionType::MoveTo, int2(10,33)},
		{PatrolActionType::LookLeft, 5.f},
		{PatrolActionType::MoveTo, int2(2,33)},
		{PatrolActionType::LookRight, 5.f},
	};

	PatrolAction patrolF[]{
		{PatrolActionType::MoveTo, int2(30,53)},
		{PatrolActionType::MoveTo, int2(30,62)},
		{PatrolActionType::MoveTo, int2(38,62)},
		{PatrolActionType::LookDown, 5.f},
		{PatrolActionType::MoveTo, int2(41,62)},
		{PatrolActionType::LookLeft, 5.f},
		{PatrolActionType::MoveTo, int2(41,53)},
	};

	PatrolAction patrolG[]{
		{PatrolActionType::MoveTo, int2(15,34)},
		{PatrolActionType::LookUp, 5.f},
		{PatrolActionType::MoveTo, int2(34,34)},
		{PatrolActionType::LookRight, 5.f},
		{PatrolActionType::MoveTo, int2(34,36)},
		{PatrolActionType::MoveTo, int2(18,36)},
		{PatrolActionType::MoveTo, int2(18,44)},
		{PatrolActionType::LookDown, 5.f},
		{PatrolActionType::MoveTo, int2(21,44)},
		{PatrolActionType::MoveTo, int2(21,46)},
		{PatrolActionType::MoveTo, int2(15,46)},
	};

	PatrolAction patrolH[]{
		{PatrolActionType::MoveTo, int2(33,36)},
		{PatrolActionType::LookLeft, 5.f},
		{PatrolActionType::MoveTo, int2(38,36)},
		{PatrolActionType::LookRight, 5.f}
	};

	PatrolAction patrolI[]{
		{PatrolActionType::MoveTo, int2(39,38)},
		{PatrolActionType::LookLeft, 5.f},
		{PatrolActionType::MoveTo, int2(46,38)},
		{PatrolActionType::MoveTo, int2(46,45)},
		{PatrolActionType::MoveTo, int2(37,45)},
		{PatrolActionType::MoveTo, int2(46,45)},
		{PatrolActionType::MoveTo, int2(46,38)},
	};

	PatrolAction patrolJ[]{
		{PatrolActionType::MoveTo, int2(68,45)},
		{PatrolActionType::LookUp, 5.f},
		{PatrolActionType::MoveTo, int2(49,45)},
		{PatrolActionType::MoveTo, int2(49,33)},
		{PatrolActionType::MoveTo, int2(66,33)},
		{PatrolActionType::MoveTo, int2(61,33)},
		{PatrolActionType::LookUp, 5.f},
		{PatrolActionType::MoveTo, int2(49,33)},
		{PatrolActionType::MoveTo, int2(49,45)},
	};

	Enemy* e = objectPool.AddEnemy(float2{0.f});
	e->CreatePatrolList(13, patrolA);
	e->SetPosition(float2{ static_cast<float>(patrolA[0].tilePosition.x),static_cast<float>(patrolA[0].tilePosition.y) } * static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(16, patrolB);
	e->SetPosition(float2{ static_cast<float>(patrolB[0].tilePosition.x),static_cast<float>(patrolB[0].tilePosition.y) } * static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());


	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(20, patrolC);
	e->SetPosition(float2{ static_cast<float>(patrolC[0].tilePosition.x),static_cast<float>(patrolC[0].tilePosition.y) } * static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(16, patrolD);
	e->SetPosition(float2{ static_cast<float>(patrolD[0].tilePosition.x),static_cast<float>(patrolD[0].tilePosition.y) }*static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(11, patrolE);
	e->SetPosition(float2{ static_cast<float>(patrolE[0].tilePosition.x),static_cast<float>(patrolE[0].tilePosition.y) }* static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(7, patrolF);
	e->SetPosition(float2{ static_cast<float>(patrolF[0].tilePosition.x),static_cast<float>(patrolF[0].tilePosition.y) }* static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(11, patrolG);
	e->SetPosition(float2{ static_cast<float>(patrolG[0].tilePosition.x),static_cast<float>(patrolG[0].tilePosition.y) }* static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(4, patrolH);
	e->SetPosition(float2{ static_cast<float>(patrolH[0].tilePosition.x),static_cast<float>(patrolH[0].tilePosition.y) }* static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(7, patrolI);
	e->SetPosition(float2{ static_cast<float>(patrolI[0].tilePosition.x),static_cast<float>(patrolI[0].tilePosition.y) }* static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());
	
	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(9, patrolJ);
	e->SetPosition(float2{ static_cast<float>(patrolJ[0].tilePosition.x),static_cast<float>(patrolJ[0].tilePosition.y) }* static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());


	// PICKUPABLES

	objectPool.AddPickupable(float2(59.f,106.f)* static_cast<float>(TERRAIN_TILE_SIZE), ItemType::Ration);
	objectPool.AddPickupable(float2(11.f,106.f)* static_cast<float>(TERRAIN_TILE_SIZE), ItemType::Pistol);
	objectPool.AddPickupable(float2(5.5f,17.f)* static_cast<float>(TERRAIN_TILE_SIZE), ItemType::Ammo);



	// TRUCK TRIGGERS

	// TRUCK 1
	level.AddRoomTeleportTrigger({ float2(57.f,86.f) * static_cast<float>(TERRAIN_TILE_SIZE),float2(59,87) * static_cast<float>(TERRAIN_TILE_SIZE) }, * player, float2(462, 919), false, true);

	level.AddRoomTeleportTrigger({ float2(55.f,120.f) * static_cast<float>(TERRAIN_TILE_SIZE), float2(65,121) * static_cast<float>(TERRAIN_TILE_SIZE) }, *player, float2(447.f, 672.f), true, true);

	// TRUCK 2
	level.AddRoomTeleportTrigger({ float2(68.f,42.f) * static_cast<float>(TERRAIN_TILE_SIZE), float2(70,43) * static_cast<float>(TERRAIN_TILE_SIZE) }, *player, float2(75.f, 916.f), false, true);

	level.AddRoomTeleportTrigger({ float2(6,120) * static_cast<float>(TERRAIN_TILE_SIZE), float2(17,121) * static_cast<float>(TERRAIN_TILE_SIZE) }, *player, float2(537.f, 320.f), true, true);


	// LEVEL TRIGGERS

	level.AddLevelTeleportTrigger({ float2(50,0) * static_cast<float>(TERRAIN_TILE_SIZE), float2(58,1) * static_cast<float>(TERRAIN_TILE_SIZE) }, *player, this, 1, float2(424.f, 727.f));
	level.AddLevelTeleportTrigger({ float2(0,0) * static_cast<float>(TERRAIN_TILE_SIZE), float2(23,1) * static_cast<float>(TERRAIN_TILE_SIZE) }, *player, this, 1, float2(64.f, 727.f));



	// CRAWL TRIGGERS
	//fences
	level.AddCrawlTrigger({ float2(65,64) * static_cast<float>(TERRAIN_TILE_SIZE), float2(67,68) * static_cast<float>(TERRAIN_TILE_SIZE) }, *player, true, 66.f * TERRAIN_TILE_SIZE, 66.f * TERRAIN_TILE_SIZE);
	level.AddCrawlTrigger({ float2(5,64) * static_cast<float>(TERRAIN_TILE_SIZE), float2(7,68) * static_cast<float>(TERRAIN_TILE_SIZE) }, *player, true, 6.f * TERRAIN_TILE_SIZE, 6.f * TERRAIN_TILE_SIZE);
	level.AddCrawlTrigger({ float2(37,64) * static_cast<float>(TERRAIN_TILE_SIZE), float2(39,68) * static_cast<float>(TERRAIN_TILE_SIZE) }, * player, true, 38.f * TERRAIN_TILE_SIZE, 38.f * TERRAIN_TILE_SIZE);
	
	// topright vent
	level.AddCrawlTrigger({ float2(61,24) * static_cast<float>(TERRAIN_TILE_SIZE), float2(63,32) * static_cast<float>(TERRAIN_TILE_SIZE) }, * player, true, 62.f * static_cast<float>(TERRAIN_TILE_SIZE), 62.f * static_cast<float>(TERRAIN_TILE_SIZE));


	//6 extra tructs


	// dialogue Triggers

	// intro dialogue
	level.AddDialogueTrigger({ float2(26,96) * static_cast<float>(TERRAIN_TILE_SIZE), float2(46,117) * static_cast<float>(TERRAIN_TILE_SIZE) }, *player, &INTRO_DIALOGUE);

	// Crawl under wall
	level.AddDialogueTrigger({ float2(52,68) * static_cast<float>(TERRAIN_TILE_SIZE), float2(71,72) * static_cast<float>(TERRAIN_TILE_SIZE) }, * player, & FENCE_DIALOGUE);


	// crawl find alternative entrance
	level.AddDialogueTrigger({ float2(25,44) * static_cast<float>(TERRAIN_TILE_SIZE), float2(44,47) * static_cast<float>(TERRAIN_TILE_SIZE) }, * player, & ENTRANCE_DIALOGUE);


}

void MetalGear::LoadLevel1()
{
	currentLevel = 1;
	level = Level(INSIDE_LEVEL_VISUAL_PATH, INSIDE_LEVEL_COLLISION_PATH, ROOM_WIDTH * 4, ROOM_HEIGHT * 4);
	level.Init(resourceManager->getTileSheet(TileSheets::InsideTerrain), resourceManager->getTileSheet(TileSheets::TerrainCollision), &dialogueManager);

	level.SetActiveRoom(int2(2, 3));
	player->SetPosition(float2(54.f * static_cast<float>(TERRAIN_TILE_SIZE), 90.f * static_cast<float>(TERRAIN_TILE_SIZE)));


	Elevator* elev = objectPool.AddElevator(float2(303, 324));
	level.AddElevatorTrigger({ {38.f*TERRAIN_TILE_SIZE, 41.f * TERRAIN_TILE_SIZE},{42.f * TERRAIN_TILE_SIZE, 43.f * TERRAIN_TILE_SIZE+7} }, *player, elev, this);


	PatrolAction patrolA[]{
		{PatrolActionType::MoveTo, int2(35,75)},
		{PatrolActionType::LookDown, 5.f},
		{PatrolActionType::MoveTo, int2(20,75)},
		{PatrolActionType::MoveTo, int2(20,86)},
		{PatrolActionType::MoveTo, int2(43,86)},
		{PatrolActionType::MoveTo, int2(43,75)},
	};

	PatrolAction patrolB[]{
		{PatrolActionType::MoveTo, int2(67,86)},
		{PatrolActionType::MoveTo, int2(20,86)},
		{PatrolActionType::LookUp, 5.f},
		{PatrolActionType::MoveTo, int2(4,86)},
		{PatrolActionType::MoveTo, int2(4,74)},
		{PatrolActionType::MoveTo, int2(67,74)},
	};

	PatrolAction patrolC[]{
		{PatrolActionType::MoveTo, int2(2,71)},
		{PatrolActionType::MoveTo, int2(2,11)},
		{PatrolActionType::MoveTo, int2(49,11)},
		{PatrolActionType::MoveTo, int2(49,7)},
		{PatrolActionType::MoveTo, int2(69,7)},
		{PatrolActionType::MoveTo, int2(69,58)},
		{PatrolActionType::MoveTo, int2(66,58)},
		{PatrolActionType::MoveTo, int2(66,71)},
	};

	PatrolAction patrolD[]{
		{PatrolActionType::MoveTo, int2(58,50)},
		{PatrolActionType::MoveTo, int2(58,18)},
		{PatrolActionType::LookUp, 5.f},
		{PatrolActionType::MoveTo, int2(22,18)},
		{PatrolActionType::MoveTo, int2(22,14)},
		{PatrolActionType::MoveTo, int2(10,14)},
		{PatrolActionType::MoveTo, int2(10,30)},
		{PatrolActionType::MoveTo, int2(6,30)},
		{PatrolActionType::MoveTo, int2(6,57)},
		{PatrolActionType::MoveTo, int2(13,57)},
		{PatrolActionType::MoveTo, int2(13,61)},
		{PatrolActionType::MoveTo, int2(45,61)},
		{PatrolActionType::MoveTo, int2(45,65)},
		{PatrolActionType::MoveTo, int2(61,65)},
		{PatrolActionType::MoveTo, int2(61,50)},
	};

	PatrolAction patrolE[]{
		{PatrolActionType::MoveTo, int2(58,36)},
		{PatrolActionType::MoveTo, int2(58,10)},
		{PatrolActionType::MoveTo, int2(65,10)},
		{PatrolActionType::MoveTo, int2(65,36)},
	};

	PatrolAction patrolF[]{
		{PatrolActionType::MoveTo, int2(57,38)},
		{PatrolActionType::LookRight, 5.f},
		{PatrolActionType::MoveTo, int2(46,38)},
		{PatrolActionType::MoveTo, int2(46,30)},
		{PatrolActionType::MoveTo, int2(26,30)},
		{PatrolActionType::MoveTo, int2(26,45)},
		{PatrolActionType::MoveTo, int2(45,45)},
		{PatrolActionType::MoveTo, int2(45,38)},
	};


	PatrolAction patrolG[]{
		{PatrolActionType::MoveTo, int2(30,5)},
		{PatrolActionType::LookDown, 5.f},
		{PatrolActionType::MoveTo, int2(41,5)},
		{PatrolActionType::LookDown, 5.f},
	};

	Enemy* e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(6, patrolA);
	e->SetPosition(float2{static_cast<float>(patrolA[0].tilePosition.x),static_cast<float>(patrolA[0].tilePosition.y)} * static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(6, patrolB);
	e->SetPosition(float2{ static_cast<float>(patrolB[0].tilePosition.x),static_cast<float>(patrolB[0].tilePosition.y) } * static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(8, patrolC);
	e->SetPosition(float2{ static_cast<float>(patrolC[0].tilePosition.x),static_cast<float>(patrolC[0].tilePosition.y) } * static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2 { 0.f });
	e->CreatePatrolList(15, patrolD);
	e->SetPosition(float2{ static_cast<float>(patrolD[0].tilePosition.x),static_cast<float>(patrolD[0].tilePosition.y) } * static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(4, patrolE);
	e->SetPosition(float2{ static_cast<float>(patrolE[0].tilePosition.x),static_cast<float>(patrolE[0].tilePosition.y) }* static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());


	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(8, patrolF);
	e->SetPosition(float2{ static_cast<float>(patrolF[0].tilePosition.x),static_cast<float>(patrolF[0].tilePosition.y) }* static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());

	e = objectPool.AddEnemy(float2{ 0.f });
	e->CreatePatrolList(4, patrolG);
	e->SetPosition(float2{ static_cast<float>(patrolG[0].tilePosition.x),static_cast<float>(patrolG[0].tilePosition.y) }* static_cast<float>(TERRAIN_TILE_SIZE) - e->GetFeetPosition());



	// triggers

	level.AddLevelTeleportTrigger({ float2(53,95) * static_cast<float>(TERRAIN_TILE_SIZE), float2(57,96) * static_cast<float>(TERRAIN_TILE_SIZE) }, * player, this, 0, float2(421, 7.f));
	level.AddLevelTeleportTrigger({ float2(9,95) * static_cast<float>(TERRAIN_TILE_SIZE), float2(11,96) * static_cast<float>(TERRAIN_TILE_SIZE) }, * player, this, 0, float2(69.f, 10.f));

	//

	level.AddCrawlTrigger({ float2(54,92) * static_cast<float>(TERRAIN_TILE_SIZE), float2(56,95)* static_cast<float>(TERRAIN_TILE_SIZE) }, * player, true, 55.f * TERRAIN_TILE_SIZE, 55.f * TERRAIN_TILE_SIZE);

	level.AddCrawlTrigger({ float2(9,92) * static_cast<float>(TERRAIN_TILE_SIZE), float2(11,95) * static_cast<float>(TERRAIN_TILE_SIZE) }, * player, true, 10.f * TERRAIN_TILE_SIZE, 10.f * TERRAIN_TILE_SIZE);


	// dialogue

	level.AddDialogueTrigger({ float2(71,79) * static_cast<float>(TERRAIN_TILE_SIZE), float2(96,96) * static_cast<float>(TERRAIN_TILE_SIZE) }, * player, & HOLLY_DIALOGUE);


}

void MetalGear::UpdateMainMenu(float)
{
	if (Engine::GetInstance().GetInput().IsKeyJustDown(GLFW_KEY_UP) || Engine::GetInstance().GetInput().IsKeyJustDown(GLFW_KEY_DOWN))
	{
		menuOption++;
		menuOption %= 2;
	}
	
	if (Engine::GetInstance().GetInput().IsKeyJustDown(GLFW_KEY_ENTER))
	{
		if (menuOption == 1)
		{
			ReadGameData();
		}


		music->PlayTrack("assets/Sounds/normalMusic.ogg");
		menuState = MenuState::Game;
	}

	textbox.SetSize(int2(89, 28));
	if (menuOption == 0)
	{
		textbox.SetText("[NEW GAME]\n CONTINUE");
	}
	else
	{
		textbox.SetText(" NEW GAME\n[CONTINUE]");
	}


}

void MetalGear::DrawMainMenu(Surface& target) const
{
	target.Clear(0xff060606);
	//target.Bar(0, 0, target.GetWidth()-1, target.GetHeight()-1, 0xff000000);
	mainMenuTexture->DrawTile(target, int2(14,8), 0);

	textbox.Draw(target, int2((GAME_WINDOW_WIDTH- 89)/2, 128));
	textbox.GetFont()->DrawText(target, "remade by neokke", int2((GAME_WINDOW_WIDTH - 16* textbox.GetFont()->GetCharacterSize().x) / 2, 94));

}

void MetalGear::UpdateDeathMenu(float)
{
	textbox.SetSize(int2(89, 28));

	if (menuOption == 0)
	{
		textbox.SetText("[CONTINUE]\n EXIT");
	}
	else
	{
		textbox.SetText(" CONTINUE\n[EXIT]");
	}

	if (Engine::GetInstance().GetInput().IsKeyJustDown(GLFW_KEY_UP) || Engine::GetInstance().GetInput().IsKeyJustDown(GLFW_KEY_DOWN))
	{
		menuOption++;
		menuOption %= 2;
	}

	if (Engine::GetInstance().GetInput().IsKeyJustDown(GLFW_KEY_ENTER))
	{
		if (menuOption == 0)
		{
			ReadGameData();
			menuState = MenuState::Game;
		}
		else
		{
			exitGame = true;
		}
		
	}
}

void MetalGear::DrawDeathMenu(Surface& target) const
{
	target.Bar(0, 0, target.GetWidth() - 1, target.GetHeight() - 1, 0);

	textbox.GetFont()->DrawText(target, "YOU DIED!", int2((GAME_WINDOW_WIDTH - 9* textbox.GetFont()->GetCharacterSize().x) / 2, 48));
	textbox.Draw(target, int2((GAME_WINDOW_WIDTH - 89) / 2, 108));

}

void MetalGear::UpdateVictoryMenu(float)
{
	textbox.SetSize(int2(57, 20));
	textbox.SetText("[EXIT]");

	if (Engine::GetInstance().GetInput().IsKeyJustDown(GLFW_KEY_ENTER))
	{
		exitGame = true;
	}
}

void MetalGear::DrawVictoryMenu(Surface& target) const
{
	target.Bar(0, 0, target.GetWidth() - 1, target.GetHeight() - 1, 0);

	textbox.GetFont()->DrawText(target, "YOU REACHED THE END OF THE GAME!", int2((GAME_WINDOW_WIDTH - 32 * textbox.GetFont()->GetCharacterSize().x) / 2, 28));
	
	textbox.GetFont()->DrawText(target, "THANKS FOR PLAYING!", int2((GAME_WINDOW_WIDTH - 19 * textbox.GetFont()->GetCharacterSize().x) / 2, 38));


	textbox.GetFont()->DrawText(target, "made by neokke!", int2((GAME_WINDOW_WIDTH - 15 * textbox.GetFont()->GetCharacterSize().x) / 2, 94));

	textbox.GetFont()->DrawText(target, "assets ripped by:\n   mr.beacon00\n   deathbringer", int2((GAME_WINDOW_WIDTH - 17 * textbox.GetFont()->GetCharacterSize().x) / 2, 132));

	textbox.GetFont()->DrawText(target, "special thanks to:\n     knnyism\n     twenmod\n       seb", int2((GAME_WINDOW_WIDTH - 18 * textbox.GetFont()->GetCharacterSize().x) / 2, 168), int2(0,1));

	textbox.Draw(target, int2((GAME_WINDOW_WIDTH - 57) / 2, 68));
}
