#include "precomp.h"
#include "resourceManager.h"

#include "aabb.h"
#include "gameobject.h"

#include "arrayList.h"

#include "entity.h"
#include "pathfinder.h"
#include "textBox.h"
#include "enemy.h"
#include "player.h"
#include "objectPool.h"

#include "tilemap.h"
#include "level.h"

#include "bullet.h"



Bullet::Bullet(float2 direction, bool ownedByPlayer)
	: GameObject(AABB(float2{0.f},float2{2.f}))
	, direction{normalize(direction)}
	, ownedByPlayer{ownedByPlayer}
{
}

void Bullet::Init(const ResourceManager& resourceManager, Level* _level, ObjectPool* _objectPool, irrklang::ISoundEngine* _soundEngine)
{
	GameObject::Init(resourceManager.getTileSheet(TileSheets::Bullet), _level, _soundEngine);
	objectPool = _objectPool;
}

void Bullet::Update(float deltaTime)
{
	SetPosition(GetPosition() + direction * BULLET_SPEED * deltaTime);
	lifeTime += deltaTime;


	Entity* hitCollider = nullptr;
	if (ownedByPlayer)
		hitCollider = objectPool->GetFirstEnemyCollider(GetCollider());
	else
		hitCollider = objectPool->GetFirstPlayerCollider(GetCollider());


	if (hitCollider)
	{
		// here we can do pixel collision
		if (PixelCollision(*hitCollider))
		{
			hitCollider->TakeDamage(BULLET_DAMAGE);
			MarkForDelete();
		}
	}
	

	float2 terrainPosition = GetPosition() + float2(0.f, static_cast<float>(BULLET_TERRAIN_HITBOX_OFFSET));
	int2 tilePos = int2(static_cast<int>(terrainPosition.x / TERRAIN_TILE_SIZE), static_cast<int>(terrainPosition.y / TERRAIN_TILE_SIZE));

	int2 tilePosHigh = int2(static_cast<int>(GetPosition().x / TERRAIN_TILE_SIZE), static_cast<int>(GetPosition().y / TERRAIN_TILE_SIZE));

	if (lifeTime > BULLET_MAX_LIFETIME || (!level->IsWalkable(level->GetColliderTile(tilePos)) && !level->IsWalkable(level->GetColliderTile(tilePosHigh))))
	{
		MarkForDelete();
	}

}

void Bullet::DrawDebug(Surface& target, int2 offset) const
{
	GetCollider().draw(target, level->GetWorldOffset() + offset,  0xffff0000);

	GetCollider().draw(target, level->GetWorldOffset() + offset + int2(0, BULLET_TERRAIN_HITBOX_OFFSET), 0xff0000ff);



}
