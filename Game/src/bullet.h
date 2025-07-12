#pragma once
#include "gameobject.h"

class ResourceManager;
class TileSheet;
class Level;
class ObjectPool;


class Bullet : public GameObject
{
public:
	Bullet(float2 direction, bool ownedByPlayer);

	void Init(const ResourceManager& resourceManager, Level* level, ObjectPool* objectPool, irrklang::ISoundEngine* soundEngine);
	void Update(float deltaTime) override;

	void DrawDebug(Surface&, int2 offset) const;

	int GetSortingOffset() const override { return BULLET_TERRAIN_HITBOX_OFFSET; }

private:
	float2 direction{ 0.f, 1.f };

	ObjectPool* objectPool;

	bool ownedByPlayer{false};

	float lifeTime{ 0.f };

};

