#pragma once


struct AABB;
class ResourceManager;

enum class Direction { Up, Down, Left, Right };

class Entity : public GameObject
{
public:
	Entity();
	Entity(const AABB& collider, int maxHealth);

	virtual ~Entity();



	void Init(TileSheet* tileSheet, 
		Level* level, 
		ObjectPool* objectPool,
		irrklang::ISoundEngine* soundEngine);

	void Init(const ResourceManager& resourceManager, 
		Level* level, 
		ObjectPool* objectPool,
		irrklang::ISoundEngine* soundEngine);


	void Draw(Surface& target, int2 offset) const override;
	
	
	virtual void TakeDamage(int damage);
	

	void SetVelocity(const float2& newVelocity) { velocity = newVelocity; }
	const float2& GetVelocity() const { return velocity; }


	int GetMaxHealth() const;
	void SetMaxHealth(int value);

	int GetCurrentHealth() const;
	void SetCurrentHealth(int value);

	uint GetCurrentTileIndex() const override;

protected:
	virtual void OnDeath();
	float2 velocity{0.f};

	AnimatedSprite* sprite{ nullptr };
	ObjectPool* objectPool{ nullptr };

	Direction lookDirection{ Direction::Down };

	int maxHealth{0};
	int health{0};


};

