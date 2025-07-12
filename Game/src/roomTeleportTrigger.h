#pragma once

class LevelTrigger;
class ObjectPool;

class RoomTeleportTrigger : public LevelTrigger
{
public:
	RoomTeleportTrigger() = delete;
	RoomTeleportTrigger(const AABB& aabb, Level& level, Player& player, bool crawlActivatable, float2 teleportPos, bool forceUncrawl);

	void ActivateTrigger() override;


private:
	float2 teleportPosition{ 0.f };
	int2 roomPosition{ 0 };
	bool crawlActivatable{ true };
	bool forceUncrawl{ true };

};

