#pragma once

class EnemySystem;
class ResourceManager;
class TileSheet;

enum class PatrolActionType
{
	Nothing,

	MoveTo,

	IdleDown,
	IdleLeft,
	IdleUp,
	IdleRight,

	LookDown,
	LookLeft,
	LookUp,
	LookRight
};




struct PatrolAction
{
	PatrolAction() {}

	PatrolAction(PatrolActionType type, float value)
		: type{ type }
		, value{ value } {}

	PatrolAction(PatrolActionType type, int2 positition)
		: type{ type }
		, tilePosition{ positition } {}

	PatrolActionType type{ PatrolActionType::Nothing };
	float value{ 0.f };	// used for distance or time
	int2 tilePosition{ 0 }; // used for moveToAction
};


enum class EnemyType
{
	Patrol,
	Search,
};


class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();

	void Init(const ResourceManager& resourceManager, Level* _level, ObjectPool* _objectPool, EnemySystem* _enemySystem, Player* player, irrklang::ISoundEngine* soundEngine);

	void CreatePatrolList(int length, PatrolAction* list);

	void TakeDamage(int damage) override;

	void Update(float deltaTime);

	void Draw(Surface& target, int2 offest) const override;

	void DrawDebug(Surface& target, int2 offset) const override;

	void OnDeath() override;

	void SetPosition(float2 newPosition);

	bool IsPlayerVisible() const { return playerVisible; }

	bool FollowPath(float deltaTime);

	void SetEnemyType(EnemyType value) { enemyType = value; }
	EnemyType GetEnemyType() const { return enemyType; }

	float2 GetFeetPosition() const;
	int2 GetTileFeetPosition() const;

	void SetPath(const ArrayList<int2>& path);

	void SetConfused(int2 soundPositionTile);

	void ShowEnemyIcon(float duration, int frame);

	void ShowTextBox(float duration, const char* text);
private:
	float CalculateVelocityMagnitude(float deltaTime, float position) const;

	void UpdatePatrol(float deltaTime);
	void UpdateSearch(float deltaTime);
	void UpdateAnimationState();

	void CheckPlayerVisibility(Player& player);

	void ShootToPlayer();

	float2 GetBulletSpawnPosition();

		
	float2 viewDirection{ 0.f,1.f };
	int2 oldPosition{ 0 };

	float confusedTime = 0.f;
	bool confusedFinished{ true };
	float confusedLookTime = 0.f;
	

	EnemyType enemyType{ EnemyType::Search };
	EnemySystem* enemySystem{ nullptr };
	Player* player{ nullptr };

	TileSheet* enemyIcon{ nullptr };
	int enemyIconFrame{ 0 };
	float enemyIconShowTime{ 0.f };
	
	TextBox textBox;
	float textBoxShowTime{ 0.f };

	float moveSpeed{ 35.f };

	PatrolAction* patrolList{ nullptr };
	int patrolListLength{ -1 };
	int currentAction{ 0 };
	float currentValue{ 0.f };


	bool playerVisible{ false };
	bool shootRange{ false };

	Pathfinder pathfinder;
	ArrayList<int2> pathNodes;
	uint currentPathIndex = 0;


	float shootCountdown{ ENEMY_SHOOT_DELAY/2};

	float stunnedDuration{ 0.f };
};

