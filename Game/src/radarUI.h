#pragma once
#include "Core/Timer.h"

class EnemySystem;
class Level;
class Player;
class ResourceManager;

class RadarUI :  public UIBase
{
public:
	RadarUI() = default;
	void Init(const ResourceManager& resourceManager,Level* level, Player* player, EnemySystem* enemySystem);

	void Update(float deltaTime);
	void Draw(Surface& target, int2 offset) const;




private:
	void DrawTerrain(Surface& target, int2 offset) const;
	void DrawInfoScreen(Surface& target, int2 offset) const;
	void DrawRadarSection(Surface& target, int2 tileOffset, int2 offset) const;

	void DrawScanLine(Surface& target, int2 offset) const;

	// render positions
	int2 alarmLevelPosition{ 37,22 };

	int2 radarInfo1{ 13, 43 };	//value of your health or time remaining position
	int2 radarInfo2{ 20, 43 };

	int2 indicatorTopLeft{ 2 };
	int2 indicatorBottomRight{ 32 };


	Level* level{ nullptr };
	EnemySystem* enemySystem{nullptr};
	Player* player{ nullptr };

	Font* radarNumbers;
	TileSheet* radarSheet;
	TileSheet* radarIndicator;
	TileSheet* radarDecoration;
	// states

	enum class BlinkerState
	{
		EmptyLeft,
		Left,
		EmptyRight,
		Right
	};

	BlinkerState blinker = BlinkerState::EmptyLeft;

	float scanLineSpeed = 50.f;
	float scanLineX = 0.f;
	
	// Timings
	
	Timer blinkTimer;
	float blinkTime = 1;


};

