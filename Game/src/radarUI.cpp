#include "precomp.h"

#include "resourceManager.h"
#include "arrayList.h"

#include "tilesheet.h"
#include "font.h"

#include "tilemap.h"
#include "level.h"

#include "aabb.h"
#include "gameobject.h"
#include "entity.h"

#include "textBox.h"
#include "pathfinder.h"
#include "enemy.h"
#include "arrayList.h"
#include "player.h"

#include "enemySystem.h"

#include "UIBase.h"
#include "radarUI.h"

void RadarUI::Init(const ResourceManager& resourceManager, Level* _level, Player* _player, EnemySystem* _enemySystem)
{
	level = _level;
	enemySystem = _enemySystem;
	player = _player;

	radarNumbers = resourceManager.getFont(Fonts::RadarNumbers);
	radarSheet = resourceManager.getTileSheet(TileSheets::Radar);
	radarDecoration = resourceManager.getTileSheet(TileSheets::RadarDecoration);
	radarIndicator = resourceManager.getTileSheet(TileSheets::RadarIndicator);
}

void RadarUI::Update(float deltaTime)
{
	if (blinkTimer.elapsed() >= blinkTime)
	{
		blinkTimer.reset();

		blinker = static_cast<BlinkerState>((static_cast<int>(blinker) + 1) % 4);	// <-- feels a bit magicy

	}

	
	scanLineX += scanLineSpeed * deltaTime;

	if (static_cast<int>(scanLineX) > (static_cast<int>(radarSheet->GetCellSize().x) - 3))
	{
		scanLineX = static_cast<float>(radarSheet->GetCellSize().x - 3);
		scanLineSpeed *= -1;
	}
	else if (static_cast<int>(scanLineX) < 0)
	{
		scanLineX = 0.f;
		scanLineSpeed *= -1;
	}
}

void RadarUI::Draw(Surface& target, int2 offset) const
{
	switch (enemySystem->GetAlarmState())
	{
	case AlarmState::Normal:
		DrawTerrain(target, offset);
		break;
	case AlarmState::Activated:
	case AlarmState::Evade:
		DrawInfoScreen(target, offset);
		break;
	}

	radarDecoration->DrawTile(target, offset+int2(1, radarSheet->GetCellSize().y+2), 0);
}

void RadarUI::DrawTerrain(Surface& target, int2 offset) const
{
	radarSheet->DrawTile(target, offset, 0);

	int2 tileOffset = int2(level->GetActiveRoom().x * ROOM_WIDTH, level->GetActiveRoom().y * ROOM_HEIGHT);
	

	bool searchingEnemiesActive = false;
	for (uint i = 0; i < enemySystem->GetEnemyList().size(); i++)
	{
		Enemy* e = enemySystem->GetEnemyList().at(i);
		if (e->GetEnemyType() == EnemyType::Search)
		{
			searchingEnemiesActive = true;
			break;
		}
	}

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			int2 currentTileOffset = tileOffset + int2(ROOM_WIDTH * (x - 1), ROOM_HEIGHT * (y - 1));
			DrawRadarSection(target, currentTileOffset, int2(19 * x + 1, 19 * y + 1) + offset);

			int2 playerTilePositon = player->GetTileFeetPosition() - currentTileOffset;
			if (playerTilePositon.x >= 0 && playerTilePositon.x < 24 && playerTilePositon.y >= 0 && playerTilePositon.y < 24)
			{
				target.Plot(static_cast<int>(playerTilePositon.x / 24.f * 18 + offset.x + 19 * x + 1), static_cast<int>(playerTilePositon.y / 24.f * 18 + offset.y + 19 * y + 1), 0xff0000ff);
			}

			if (!searchingEnemiesActive)
			{
				for (uint i = 0; i < enemySystem->GetEnemyList().size(); i++)
				{
					int2 tilePositon = enemySystem->GetEnemyList().at(i)->GetTileFeetPosition() - currentTileOffset;
					if (tilePositon.x >= 0 && tilePositon.x < 24 && tilePositon.y >= 0 && tilePositon.y < 24)
					{
						target.Plot(static_cast<int>(tilePositon.x / 24.f * 18 + offset.x + 19 * x + 1), static_cast<int>(tilePositon.y / 24.f * 18 + offset.y + 19 * y + 1), 0xffffffff);
					}
				}
			}

			
		}
	}


	
		

	
	if (searchingEnemiesActive)
	{
		DrawScanLine(target, offset);
	}
	

}


void RadarUI::DrawInfoScreen(Surface& target, int2 offset) const
{
	bool alarmActive = enemySystem->GetAlarmState() == AlarmState::Activated;

	int radarView = alarmActive ? 2 : 1;
	radarSheet->DrawTile(target, offset, radarView);
		
	radarNumbers->DrawTile(target, enemySystem->GetAlarmLevel(), alarmLevelPosition + offset);

	// value1 indicates the 10th position of the digit
	// value1 indicates the 1th position of the digit
	int value1 = 9; 
	int value2 = 9; 

	if (alarmActive)
	{
		if (player->GetCurrentHealth() >= 0)
		{
			value1 = player->GetCurrentHealth() / 10;
			value2 = player->GetCurrentHealth() % 10;
		}
		else
		{
			value1 = 0;
			value2 = 0;
		}

	}
	else
	{
		int timeRemaining = static_cast<int>(enemySystem->GetEvadeTimeRemaining());
		value1 = timeRemaining / 10;
		value2 = timeRemaining % 10;

	}

	radarNumbers->DrawTile(target, value1, radarInfo1 + offset);
	radarNumbers->DrawTile(target, value2, radarInfo2 + offset);

	
	switch (blinker)
	{
	case RadarUI::BlinkerState::EmptyLeft:
	case RadarUI::BlinkerState::EmptyRight:
		
		break;
	case RadarUI::BlinkerState::Left:
		radarIndicator->DrawTile(target, indicatorTopLeft + offset, alarmActive? 3 : 1);
		break;
	case RadarUI::BlinkerState::Right:
		radarIndicator->DrawTile(target, indicatorBottomRight + offset, alarmActive ? 2 : 0);
		break;
	default:
		break;
	}


}

void RadarUI::DrawRadarSection(Surface& target, int2 tileOffset, int2 offset) const
{

	for (int y = 0; y < 24; y++)	// radar sections are 18x18
	{
		for (int x = 0; x < 24; x++)
		{

			switch (level->GetColliderTile(tileOffset + int2(x,y)))
			{
			case CollisionTileType::WALKABLE_OPEN:
				target.Plot(static_cast<int>(x/ float(ROOM_WIDTH) *18) + offset.x, static_cast<int>(y / float(ROOM_HEIGHT) * 18) + offset.y, 0xff060606);
				break;

			default:
				break;
			}
		}
	}

}

void RadarUI::DrawScanLine(Surface& target, int2 offset) const
{
	int xDraw = static_cast<int>(scanLineX) + offset.x + 1;

	target.Line(xDraw, offset.y + 1, xDraw, offset.y + radarSheet->GetTileSize().y - 2, 0xff0060ad);
}
