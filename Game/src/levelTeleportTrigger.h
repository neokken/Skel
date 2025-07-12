#pragma once
#include "levelTrigger.h"

class MetalGear;

class LevelTeleportTrigger : public LevelTrigger
{
public:
	LevelTeleportTrigger() = delete;
	LevelTeleportTrigger(const AABB& aabb, Level& level, Player& player, bool crawlActivatable, MetalGear* game, int levelIndex, float2 teleportPos, bool forceUnCrawl);

	void ActivateTrigger() override;


private:
	float2 teleportPosition{ 0.f };
	int2 roomPosition{ 0 };
	int levelIndex{ 0 };
	bool forceUnCrawl{ false };
	
	MetalGear* game{nullptr};
};
