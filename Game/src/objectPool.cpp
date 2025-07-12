#include "precomp.h"

#include "aabb.h"
#include "gameObject.h"
#include "entity.h"

#include "arrayList.h"
#include "pathfinder.h"

#include "player.h"

#include "textBox.h"
#include "enemy.h"
#include "bullet.h"
#include "elevator.h"

#include "pickupable.h"

#include "objectPool.h"




ObjectPool::~ObjectPool()
{
	for (uint i = 0; i < objects.size(); i++)
	{
		delete objects.at(i);
	}
}

void ObjectPool::Init(Level* _level, EnemySystem* _enemySystem, ResourceManager* _resourceManager, irrklang::ISoundEngine* _soundEngine)
{

	level = _level;
	enemySystem = _enemySystem;
	resourceManager = _resourceManager;
	soundEngine = _soundEngine;
}

Enemy* ObjectPool::AddEnemy(float2 worldPosition)
{
	Enemy* newEnemy = new Enemy();
	newEnemy->Init(*resourceManager, level, this, enemySystem, player, soundEngine);
	newEnemy->SetPosition(worldPosition);

	
	objects.append(newEnemy);

	return newEnemy;
}

Player* ObjectPool::AddPlayer(float2 worldPosition, Inventory* inventory)
{
	Player* newPlayer = new Player();
	newPlayer->Init(*resourceManager, level,this, inventory, enemySystem, soundEngine);
	newPlayer->SetPosition(worldPosition);

	objects.append(newPlayer);

	player = newPlayer;

	return newPlayer;
}

Bullet* ObjectPool::AddBullet(float2 worldPosition, float2 direction, bool ownedByPlayer)
{
	Bullet* newBullet = new Bullet(direction, ownedByPlayer);
	newBullet->Init(*resourceManager, level, this, soundEngine);
	newBullet->SetPosition(worldPosition);

	objects.append(newBullet);

	return newBullet;
}

Pickupable* ObjectPool::AddPickupable(float2 worldPosition, ItemType itemType)
{

	Pickupable* newPickupable = new Pickupable(itemType);
	newPickupable->Init(*resourceManager, level, soundEngine);
	newPickupable->SetPosition(worldPosition);

	objects.append(newPickupable);

	return newPickupable;
}

Elevator* ObjectPool::AddElevator(float2 worldPosition)
{
	Elevator* newElevator = new Elevator();
	newElevator->Init(*resourceManager, level, this, soundEngine);
	newElevator->SetPosition(worldPosition);


	objects.append(newElevator);

	return newElevator;
}

void ObjectPool::ClearForLevel()
{
	uint currentIndex = 0;
	while (currentIndex < objects.size())
	{
		Player* p = dynamic_cast<Player*>(objects.at(currentIndex));

		if (p == nullptr)
		{
			DeleteObject(objects.at(currentIndex));
		}
		else
		{
			currentIndex++;
		}
	}
}

void ObjectPool::Update(float deltaTime)
{
	bool isMarkedDelete = false;

	for (uint i = 0; i < objects.size(); i++)
	{
		objects.at(i)->Update(deltaTime);
		isMarkedDelete = isMarkedDelete || objects.at(i)->IsMarkedForDelete();
	}

	if (isMarkedDelete)
	{
		uint currentIndex = 0;
		while (currentIndex < objects.size())
		{
			if (objects.at(currentIndex)->IsMarkedForDelete())
			{
				DeleteObject(objects.at(currentIndex));
			}
			else
			{
				currentIndex++;
			}
		}
	}


}

void ObjectPool::Draw(Surface& target, int2 offset)
{
	objects.quickSort([](GameObject* left, GameObject* right)
		{int lbottom = static_cast<int>(left->GetCollider().bottom()) + left->GetSortingOffset();
		int rbottom = static_cast<int>(right->GetCollider().bottom())+ right->GetSortingOffset();
		return lbottom < rbottom; });


	for (uint i = 0; i < objects.size(); i++)
	{
		objects.at(i)->Draw(target, offset); 
	}
}

Enemy* ObjectPool::GetFirstEnemyCollider(const AABB& aabb) const
{
	for (uint i = 0; i < objects.size(); i++)
	{
		Enemy* e = dynamic_cast<Enemy*>(objects.at(i));
		if (e != nullptr && !e->IsDisabled() && e->GetCollider().intersect(aabb))
			return e;
	}
	return nullptr;
}

Entity* ObjectPool::GetFirstEntityColliderNonPlayer(const AABB& aabb) const
{
	for (uint i = 0; i < objects.size(); i++)
	{
		Entity* e = dynamic_cast<Entity*>(objects.at(i));
		if (e != nullptr && !e->IsDisabled() && e->GetCollider().intersect(aabb) && dynamic_cast<Player*>(e)==nullptr)
			return e;
	}
	return nullptr;
}

Player* ObjectPool::GetFirstPlayerCollider(const AABB& aabb) const
{
	for (uint i = 0; i < objects.size(); i++)
	{
		Player* p  = dynamic_cast<Player*>(objects.at(i));
		if (p != nullptr && !p->IsDisabled() && p->GetCollider().intersect(aabb))
			return p;
	}
	return nullptr;
}

Pickupable* ObjectPool::GetFirstPickupableCollider(const AABB& aabb) const
{
	for (uint i = 0; i < objects.size(); i++)
	{
		Pickupable* p = dynamic_cast<Pickupable*>(objects.at(i));
		if (p != nullptr && !p->IsDisabled() && p->GetCollider().intersect(aabb))
			return p;
	}
	return nullptr;
}

#ifdef OBJECT_DEBUG

void ObjectPool::DrawDebug(
	
	Surface& target, int2 offset)
{
	for (uint i = 0; i < objects.size(); i++)
	{
		GameObject* ob = objects.at(i);

#ifdef PLAYER_DEBUG
		Player* pl = dynamic_cast<Player*>(ob);
		if (pl != nullptr)
		{
			pl->DrawDebug(target, offset);
			continue;
		}

#endif // PLAYER_DEBUG


#ifdef ENEMY_DEBUG
		Enemy* enemy = dynamic_cast<Enemy*>(ob);
		if (enemy != nullptr)
		{
			enemy->DrawDebug(target, offset);
			continue;
		}

#endif // PLAYER_DEBUG

#ifdef BULLET_DEBUG
		Bullet* bullet = dynamic_cast<Bullet*>(ob);
		if (bullet != nullptr)
		{
			bullet->DrawDebug(target, offset);
			continue;
		}


#endif // BULLET_DEBUG

#ifdef PICKUPABLE_DEBUG
		Pickupable* pickup = dynamic_cast<Pickupable*>(ob);
		if (pickup != nullptr)
		{
			pickup->DrawDebug(target, offset);
			continue;
		}
#endif // PICKUPABLE_DEBUG

#ifdef ELEVATOR_DEBUG
		Elevator* elev = dynamic_cast<Elevator*>(ob);
		if (elev != nullptr)
		{
			elev->DrawDebug(target, offset);
			continue;
		}
#endif // ELEVATOR_DEBUG




	}
}
#endif

void ObjectPool::DeleteObject(GameObject* object)
{
	objects.remove(object);
	delete object;
}



