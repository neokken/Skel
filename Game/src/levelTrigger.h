#pragma once


struct AABB;
class Level;
class Player;

class LevelTrigger
{
public:
	LevelTrigger(const AABB& aabb, Level& level, Player& player, bool crawlActivatable);
	virtual ~LevelTrigger() {}


	bool CheckCollision(const AABB& other) const;

	virtual void ActivateTrigger() = 0;


	void SetPosition(float2 position) { collider.setPosition(position); }

	const AABB& GetCollider() const { return collider; }

	void DrawDebug(Surface& target, int2 offset, uint color) const;

protected:
	AABB collider;
	Player& player;
	Level& level;
	bool crawlActivatable{ true };

	
};

