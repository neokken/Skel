#pragma once


namespace skel
{
	class InputManager;
}

class ResourceManager;
class InputManager;
class Inventory;
class EnemySystem;

enum class CollisionTileType;


class Player : public Entity
{
public:
	Player();

	void Init(const ResourceManager& resourceManager, Level* level, ObjectPool* objectPool, Inventory* inventory, EnemySystem* enemySystem, irrklang::ISoundEngine* _soundEngine);

	void Reset();

	void HandleInput();
	void Update(float deltaTime);

	void Draw(Surface& target, int2 offset = { 0,0 }) const;

	void SetPosition(float2 newPosition) override;

	void CheckCollision(const AABB& otherAABB, CollisionTileType colliderType);
	void ResolveCollision(const AABB& own, const AABB& otherAABB);

	void DrawDebug(Surface& target, int2 offset) const;


	bool IsCrawling() const;
	bool IsKneeling() const;

	const AABB& GetFullCollider() const;

	const AABB& GetActiveCollider() const;

	const AABB& GetWalkAABB() const { return walkCollider; }
	const AABB& GetCrawlVCollider() const;
	const AABB& GetCrawlHCollider() const;

	void HandleKeyDown(int key);
	void HandleKeyUp(int key);
	int GetFirstKeyDown() const;


	void SetCrawl(bool value) { isCrawling = value; } // Bypases the force crawling check

	void SetFeetPosition(float2 position);
	float2 GetFeetPosition() const;
	int2 GetTileFeetPosition() const;

	bool IsForcedToCrawl() const;

	Inventory* GetInventory() const;

	void ResetCrawlDisallow();
	void SetCrawlDisallowVertical();
	void SetCrawlDisallowHorizontal();

	void SetLookDirection(Direction dir);
	Direction GetLookDirection() const;

	virtual void TakeDamage(int damage) override;

	bool IsDead() const;
	bool DeathCompleted() const;
protected:
	void OnDeath() override;
	
private:
	void SetPlayerStates();

	bool isKeyInStack(int key) const;

	void SetAnimationBasedDirection(uint UP, uint DOWN, uint LEFT, uint RIGHT);
	void UpdateAnimations();

	AABB GeneratePunchHitBox() const;


	bool isDead{ false };
	bool finishedDeath{false};

	skel::InputManager* inputManager{ nullptr };
	Inventory* inventory{ nullptr };
	EnemySystem* enemySystem{ nullptr };

	float playerSpeed = 55.f;
	float playerCrawlSpeed = 20.f;

	Direction wishDirection = Direction::Up;

	float turnTime = .2f;
	float currentTurnTime = 0.f;

	enum class PunchState {
		FirstFrame,
		Animation,
		Finished
	};
	PunchState punchState = PunchState::Finished;

	float punchCooldown = 0.f;






	int keyStack[4]{ 0 };
	int stackTop{ -1 };


	AABB walkCollider{ SNAKE_WALK_HITBOX_OFFSET,SNAKE_WALK_HITBOX_OFFSET + SNAKE_WALK_HITBOX_SIZE };

	AABB crawlHorizontalCollider{ SNAKE_CRAWLH_HITBOX_OFFSET,SNAKE_CRAWLH_HITBOX_OFFSET + SNAKE_CRAWLH_HITBOX_SIZE };

	AABB crawlVerticalCollider{ SNAKE_CRAWLV_HITBOX_OFFSET,SNAKE_CRAWLV_HITBOX_OFFSET + SNAKE_CRAWLV_HITBOX_SIZE };

	AABB fullCollider{ float2(0.f), float2(0.f) }; // will be set in the initializer
	float2 fullColliderOffset{ 0.f };


	bool isCrawling{ false };
	bool isKneeling{ false };

	bool forceCrawl{ false };
	bool visualCrawl{ false }; // this is for checking if a part is still under somethuing

	bool allowHCrawl{ false };
	bool allowVCrawl{ false };

	bool disallowVerticalCrawl{ false };
	bool disallowHorizontalCrawl{ false };

};


