#include "precomp.h"

#include "aabb.h"
#include "gameobject.h"
#include "entity.h"
#include "player.h"

#include "tilemap.h"
#include "arrayList.h"
#include "level.h"

#include "levelTrigger.h"

LevelTrigger::LevelTrigger(const AABB& aabb, Level& level, Player& player, bool crawlActivatable)
	: collider{ aabb }
	, level{ level }
	, player{ player }
	, crawlActivatable{ crawlActivatable }
{
}

bool LevelTrigger::CheckCollision(const AABB& other) const
{
	if (crawlActivatable || !player.IsCrawling() )
		return collider.intersect(other);
	return false;
}

void LevelTrigger::DrawDebug(Surface& target, int2 offset, uint color) const
{
	collider.draw(target,level.GetWorldOffset()+offset, color);
}




