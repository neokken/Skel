#include "precomp.h"

#include "resourceManager.h"

#include "tilemap.h"
#include "arrayList.h"
#include "level.h"


#include "aabb.h"
#include "gameobject.h"

#include "animatedsprite.h"

#include "entity.h"


Entity::Entity()
	: GameObject()
	, sprite{new AnimatedSprite()}
{
}

Entity::Entity(const AABB& aabb, int maxHealth)
	: GameObject(aabb)
	, maxHealth{ maxHealth }
	, health{ maxHealth }
	, sprite{ new AnimatedSprite() }
{
}

Entity::~Entity()
{
	delete sprite;
}

void Entity::Init(TileSheet* _tileSheet, Level* _level, ObjectPool* _objectPool, irrklang::ISoundEngine* _soundEngine)
{
	GameObject::Init(_tileSheet, _level, _soundEngine);
	sprite->Init(_tileSheet);
	objectPool = _objectPool;
}

void Entity::Init(const ResourceManager& resourceManager, Level* _level, ObjectPool* _objectPool, irrklang::ISoundEngine* _soundEngine)
{
	Init(resourceManager.getTileSheet(TileSheets::Empty), _level, _objectPool, _soundEngine);
}

void Entity::Draw(Surface& target, int2 offset) const
{
	if (IsDisabled() || !IsVisible())
		return;

	int2 localPosition = level->worldToScreenSpace(position);
	sprite->Draw(target, localPosition + offset);
}


void Entity::TakeDamage(int damage)
{
	health -= damage;

	if (health <= 0)
	{
		OnDeath();
	}
}

void Entity::OnDeath()
{
	MarkForDelete();
}

int Entity::GetMaxHealth() const
{
	return maxHealth;
}

void Entity::SetMaxHealth(int value)
{
	maxHealth = value;
}

int Entity::GetCurrentHealth() const
{
	return health;
}

void Entity::SetCurrentHealth(int value)
{
	health = value;
}

uint Entity::GetCurrentTileIndex() const
{
	return sprite->GetCurrentTileIndex();
}
