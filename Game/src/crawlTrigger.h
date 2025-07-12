#pragma once
#include "levelTrigger.h"

class CrawlTrigger : public LevelTrigger
{
public:
	CrawlTrigger() = delete;
	CrawlTrigger(const AABB& aabb, Level& level, Player& player, bool crawlActivatable, bool vertical, float minRange, float maxRange);

	void ActivateTrigger() override;

private:

	bool vertical{ false };
	float minRange{ 0.f };
	float maxRange{ 0.f };

};

