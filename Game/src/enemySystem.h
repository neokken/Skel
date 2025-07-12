#pragma once

class ObjectPool;
struct UpdateData;
class Enemy;
class MusicTrack;

enum class AlarmState
{
	Normal,
	Activated,
	Evade
};


class EnemySystem
{
public:
	EnemySystem() {};
	void Init(ObjectPool* objectpool, Level* level, Player* player, irrklang::ISoundEngine* soundEngine, MusicTrack* music);

	~EnemySystem();

	void Update(float deltaTime);

	void ActivateAlarm();
	AlarmState GetAlarmState() const { return alarmState; }
	void SetAlarmState(AlarmState state);

	float GetEvadeTimeRemaining() const { return remainingEvadeTime; }
	void SetEnemyGoal(Enemy* enemy) const;

	Enemy* CreateSearchingEnemy(int2 tile);

	void SoundResponse(int2 audioPositionTile);


	// only objectpool will use this function
	void AddEnemyPointer(Enemy* enemy);
	Enemy* RemoveEnemy (Enemy* enemy); // only enemy destructor will use this function
	
	uint GetAlarmLevel() const;
	void SetAlarmLevel(uint value);
	
	const ArrayList<Enemy*>& GetEnemyList() const;
private:
	void UpdateNormalState(float deltaTime);
	void UpdateEvasionState(float deltaTime);
	void UpdateAlarmState(float deltaTime);
	void SpawnEnemies(float deltaTime);


	bool PlayerVisible() const;


	ArrayList<int2> GenerateScreenEdgePositions(int tile) const;
	ArrayList<int2> GenerateRoomPositions(int margin) const;

	int2 alarmActivatedRoom{ 0 };

	ObjectPool* objectPool{ nullptr };
	Level* level{nullptr};
	Player* player{ nullptr };
	ArrayList<Enemy*> enemies;


	bool finishedNormalState = false;

	AlarmState alarmState{ AlarmState::Normal};	
	
	float remainingAlarmTime{ 0.f };
	

	float remainingEvadeTime{ 0.f };

	uint searchingEnemies{ 0 };

	float spawnDelay{ ENEMY_SPAWN_DELAY };
	

	uint alarmLevel = 5;

	irrklang::ISoundEngine* soundEngine{ nullptr };
	MusicTrack* musictrack{ nullptr };

};