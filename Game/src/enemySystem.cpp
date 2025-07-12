#include "precomp.h"

#include "arrayList.h"

#include "aabb.h"
#include "gameobject.h"
#include "entity.h"
#include "pathfinder.h"

#include "textBox.h"
#include "enemy.h"
#include "player.h"

#include "objectPool.h"

#include "tilemap.h"
#include "level.h"
#include "musicTrack.h"

#include "enemySystem.h"



void EnemySystem::Init(ObjectPool* _objectpool, Level* _level, Player* _player, irrklang::ISoundEngine* _soundEngine, MusicTrack* music)
{
	objectPool = _objectpool;
	level = _level;
	player = _player;
	soundEngine = _soundEngine;

	musictrack = music;
}

EnemySystem::~EnemySystem()
{
}

void EnemySystem::Update(float deltaTime)
{
	switch (alarmState)
	{
	case AlarmState::Normal:
		UpdateNormalState(deltaTime);
		break;
	case AlarmState::Activated:
		UpdateAlarmState(deltaTime);
		SpawnEnemies(deltaTime);
		break;
	case AlarmState::Evade:
		UpdateEvasionState(deltaTime);
		SpawnEnemies(deltaTime);
		break;
	}


	if (searchingEnemies > 0)
	{
		// Delete non visible enemies
		uint currentI = 0;
		while (currentI < enemies.size())
		{


			if (enemies.at(currentI)->GetEnemyType() == EnemyType::Search && !enemies.at(currentI)->IsVisible())
			{

				objectPool->DeleteObject(enemies.at(currentI));
				continue;

			}


			currentI++;

		}
	}


}


void EnemySystem::ActivateAlarm()
{
	// We only hide the other enemies if we were in the normal state, 
	// since in the other states they are already hidden

	if (alarmState != AlarmState::Activated)
	{
		soundEngine->play2D("assets/Sounds/Alert.ogg");
	}

	if (alarmState == AlarmState::Normal)
	{
		musictrack->PlayTrack("assets/Sounds/AlarmMusic.ogg");

		printf("Alarm\n");
		uint size = objectPool->GetSize();
		for (uint i = 0; i < size; i++)
		{
			Enemy* e = dynamic_cast<Enemy*>(objectPool->GetObjectAt(i));

			if (e != nullptr && e->GetEnemyType() == EnemyType::Patrol)
			{
				// we disable all the patrolling enemies.
				// and if they are visible on screen we spawn a searching enemy on top of it 
				// so it looks like it just switched behaviour
				if (e->IsVisible() && !e->IsDisabled())
				{
					Enemy* p = CreateSearchingEnemy(e->GetTileFeetPosition());
					p->SetEnemyType(EnemyType::Search);
					p->ShowEnemyIcon(.5f, 1);
				}
				e->SetDisabled(true);

			}
		}
	}

	finishedNormalState = false;
	alarmState = AlarmState::Activated;

	remainingAlarmTime = ALARM_TIME_LENGTH;
	remainingEvadeTime = EVADE_TIME_LENGTH;
	alarmActivatedRoom = level->GetActiveRoom();
}

void EnemySystem::SetAlarmState(AlarmState state)
{
	alarmState = state;
}

Enemy* EnemySystem::CreateSearchingEnemy(int2 tile)
{
	searchingEnemies++;

	
	int2 tileCenter = int2(tile.x * TERRAIN_TILE_SIZE + TERRAIN_TILE_SIZE / 2u, tile.y * TERRAIN_TILE_SIZE + TERRAIN_TILE_SIZE / 2u);

	uint2 enemySize = tileSheetData[static_cast<int>(TileSheets::Enemy)].cellSize;
	return objectPool->AddEnemy(float2(static_cast<float>(tileCenter.x), static_cast<float>(tileCenter.y)) - float2(static_cast<float>(enemySize.x / 2u), static_cast<float>(enemySize.y - 2u)) );
}

void EnemySystem::SoundResponse(int2 audioPositionTile)
{
	for (uint i = 0; i < enemies.size(); i++)
	{
		Enemy* enemy = enemies.at(i);


		if (!enemy->IsDisabled() && enemy->IsVisible() && (alarmState==AlarmState::Normal|| alarmState == AlarmState::Evade))
		{
			enemy->SetConfused(audioPositionTile);
		}

	}
}

void EnemySystem::AddEnemyPointer(Enemy* enemy)
{
	enemies.append(enemy);
}

Enemy* EnemySystem::RemoveEnemy (Enemy* enemy)
{
	if (enemy->GetEnemyType() == EnemyType::Search)
	{
		searchingEnemies--;
		spawnDelay = ENEMY_SPAWN_DELAY / 2;
	}

	return enemies.remove(enemy);
}

uint EnemySystem::GetAlarmLevel() const
{
	return alarmLevel;
}

void EnemySystem::SetAlarmLevel(uint value)
{
	alarmLevel = value;
}

void EnemySystem::SetEnemyGoal(Enemy* enemy) const
{
	Pathfinder p;

	if (alarmState == AlarmState::Activated)
	{
		
		ArrayList<int2> newPath = p.CalculatePath(*level, enemy->GetTileFeetPosition(), player->GetTileFeetPosition());

		if (newPath.size() == 0)
		{
			ArrayList<int2> leavePositions = GenerateRoomPositions(5); // extra extra tile so it walks outside the not destroy range. // TODO maybe needs to be 2 extra but need to test that
			if (leavePositions.size() != 0)
			{
				int rindex = RandomUInt() % leavePositions.size();

				newPath = p.CalculatePath(*level, enemy->GetTileFeetPosition(), leavePositions.at(rindex));
				// if this one also returns no valid path it just stands there for one frame and tries another random leave position next frame.
			}
			
		}

		enemy->SetPath(newPath);

	}


	else if (alarmState == AlarmState::Evade)
	{
		ArrayList<int2> randomPos = GenerateRoomPositions(5);
		if (randomPos.size() != 0)
		{
			int rindex = RandomUInt() % randomPos.size();

			randomPos = p.CalculatePath(*level, enemy->GetTileFeetPosition(), randomPos.at(rindex));
			// if this one also returns no valid path it just stands there for one frame and tries another random leave position next frame.
			enemy->SetPath(randomPos);
		}

	}



	else if (alarmState == AlarmState::Normal)
	{
		// if we are in normal state the searching enemies will walk offscreen



		ArrayList<int2> leavePositions = GenerateScreenEdgePositions(5); // extra extra tile so it walks outside the not destroy range. // TODO maybe needs to be 2 extra but need to test that

		ArrayList<int2> newPath;
		if (leavePositions.size() != 0)
		{
			int rindex = RandomUInt() % leavePositions.size();

			newPath = p.CalculatePath(*level, enemy->GetTileFeetPosition(), leavePositions.at(rindex));
			// if this one also returns no valid path it just stands there for one frame and tries another random leave position next frame.
		}

		enemy->SetPath(newPath);
		// again here if this one returns no valid path it just stands there for one frame and tries another random leave position next frame.
	}



}


const ArrayList<Enemy*>& EnemySystem::GetEnemyList() const
{
	return enemies;
}

void EnemySystem::UpdateNormalState(float)
{
	// clearing up things
	// re-enable the patrolling tiles

	if (!finishedNormalState)
	{
		finishedNormalState = true;
		for (uint i = 0; i < enemies.size(); i++)
		{
			Enemy* e = enemies.at(i);

			if (e->IsDisabled())
			{
				finishedNormalState = false;
				if (!e->IsVisible())
					e->SetDisabled(false);
			}

		
		}
	}



}

void EnemySystem::UpdateEvasionState(float deltaTime)
{
	remainingEvadeTime -= deltaTime;
	
	if (remainingEvadeTime <= 0.f)
	{
		alarmState = AlarmState::Normal;

		musictrack->PlayTrack("assets/Sounds/normalMusic.ogg");
		printf("lost them\n");
	}

}

void EnemySystem::UpdateAlarmState(float deltaTime)
{
	if (!PlayerVisible() && (alarmActivatedRoom.x != level->GetActiveRoom().x || alarmActivatedRoom.y != level->GetActiveRoom().y))
	{
		remainingAlarmTime -= deltaTime;
	}

	if (remainingAlarmTime < 0.f)
	{
		alarmState = AlarmState::Evade;
		printf("Switched to evade\n");
	}

}

void EnemySystem::SpawnEnemies(float deltaTime)
{

	if (searchingEnemies < MAX_SEARCHING_ENEMIES)
	{
		if (spawnDelay <= 0.f)
		{
			spawnDelay = ENEMY_SHOOT_DELAY;

			ArrayList<int2> spawnPositions = GenerateScreenEdgePositions(1);


			if (spawnPositions.size() != 0)
			{
				int rindex = RandomUInt() % spawnPositions.size();


				int2 spawnPos = spawnPositions.at(rindex);
				CreateSearchingEnemy(spawnPos);
			}
		}
		else
		{
			spawnDelay -= deltaTime;
		}
	}
}

bool EnemySystem::PlayerVisible() const
{
	for (uint i = 0; i < enemies.size(); i++)
	{
		if (!enemies.at(i)->IsDisabled() && enemies.at(i)->IsPlayerVisible())
		{
			return true;
		}
	}
	return false;
}


ArrayList<int2> EnemySystem::GenerateScreenEdgePositions(int tile) const
{

	ArrayList<int2> spawnPositions;

	for (int x = 0; x < 24 + 2 * tile; x++)
	{
		int xpos = (-level->GetWorldOffset().x / TERRAIN_TILE_SIZE) + x - tile;
		int ypos = (-level->GetWorldOffset().y / TERRAIN_TILE_SIZE)		- tile;

		if (level->IsWalkable(level->GetColliderTile(int2(xpos, ypos)))) {
			spawnPositions.append(int2(xpos, ypos));
		}

		ypos += 23 + tile*2;
		if (level->IsWalkable(level->GetColliderTile(int2(xpos, ypos)))) {
			spawnPositions.append(int2(xpos, ypos));
		}

	}

	for (int y = 0; y < 24 + 2 * tile; y++)
	{
		int xpos = (-level->GetWorldOffset().x / TERRAIN_TILE_SIZE)     - tile;
		int ypos = (-level->GetWorldOffset().y / TERRAIN_TILE_SIZE) + y - tile;

		if (level->IsWalkable(level->GetColliderTile(int2(xpos, ypos)))) {
			spawnPositions.append(int2(xpos, ypos));
		}

		xpos += 23 + tile*2;
		if (level->IsWalkable(level->GetColliderTile(int2(xpos, ypos)))) {
			spawnPositions.append(int2(xpos, ypos));
		}

	}

	return spawnPositions;
}

ArrayList<int2> EnemySystem::GenerateRoomPositions(int margin) const
{
	ArrayList<int2> spawnPositions;

	for (int x = -margin; x < 24 + margin; x++)
	{
		for (int y = -margin; y < 24 + margin; y++)
		{
			int xpos = (-level->GetWorldOffset().x / TERRAIN_TILE_SIZE) + x;
			int ypos = (-level->GetWorldOffset().y / TERRAIN_TILE_SIZE) + y;

			if (level->IsWalkable(level->GetColliderTile(int2(xpos, ypos)))) {
				spawnPositions.append(int2(xpos, ypos));
			}
		}
	}

	return spawnPositions;
}
