#include "precomp.h"
#include "aabb.h"
#include "gameobject.h"
#include "entity.h"
#include "player.h"

#include "crawlTrigger.h"

CrawlTrigger::CrawlTrigger(const AABB& aabb, Level& level, Player& player, bool crawlActivatable, bool vertical, float minRange, float maxRange)
	: LevelTrigger(aabb, level, player, crawlActivatable)
	, vertical {vertical}
	, minRange{minRange}
	, maxRange{maxRange}
{
}

void CrawlTrigger::ActivateTrigger() 
{
	if (!player.IsCrawling())
		return;

	float2 position = player.GetFeetPosition();

	if (vertical)
	{
		position.x = std::clamp(position.x, minRange, maxRange);
		player.SetCrawlDisallowHorizontal();
	}
	else
	{
		position.y = std::clamp(position.y, minRange, maxRange);
		player.SetCrawlDisallowVertical();
	}

	player.SetFeetPosition(position);
}
