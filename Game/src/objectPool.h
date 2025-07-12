#pragma once

class InputManager;
class ResourceManager;
class Enemy;
class Level;
class EnemySystem;
class Bullet;
class Pickupable;
class Inventory;
class Elevator;

template <class T>
class ArrayList;


class ObjectPool
{
public:
	ObjectPool() = default;
	~ObjectPool();
	
	void Init(Level* level, EnemySystem* enemySystem, ResourceManager* resourceManager, irrklang::ISoundEngine* soundEngine);

	Enemy* AddEnemy(float2 worldPosition);

	Player* AddPlayer(float2 worldPosition, Inventory* ivnventory);

	Bullet* AddBullet(float2 worldPosition, float2 direction, bool ownedByPlayer);

	Pickupable* AddPickupable(float2 worldPosition, ItemType itemType);

	Elevator* AddElevator(float2 worldPosition);

	void ClearForLevel();


	void Update(float deltaTime);
	void Draw(Surface& target, int2 offset);

	Enemy* GetFirstEnemyCollider(const AABB& aabb) const;

	Entity* GetFirstEntityColliderNonPlayer(const AABB& aabb) const;

	Player* GetFirstPlayerCollider(const AABB& aabb) const;

	Pickupable* GetFirstPickupableCollider(const AABB& aabb) const;


#ifdef OBJECT_DEBUG
	void DrawDebug(Surface& target, int2 offset);
#endif OBJECT_DEBUG

	const ArrayList<GameObject*>& GetObjects() const { return objects; };
	
	int GetSize() const { return objects.size(); }

	GameObject* GetObjectAt(int index) const { return objects.at(index); }

	void DeleteObject(GameObject* object);
		
private:
	ResourceManager* resourceManager{nullptr};
	
	Level* level{ nullptr };
	EnemySystem* enemySystem{ nullptr };
	Player* player{ nullptr };

	ArrayList<GameObject*> objects;

	irrklang::ISoundEngine* soundEngine{ nullptr };
};

 