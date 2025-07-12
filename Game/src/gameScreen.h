#pragma once

class ObjectPool;
class Level;
class EnemySystem;
class Player;
class RadarUI;

class GameScreen : public UIBase
{
public:
	void Init(Level* level, ObjectPool* objectPool, Player* player, EnemySystem* enemySystem);

	void Update(float deltaTime) override;
	void Draw(Surface& target, int2 offset = { 0, 0 }) const override;

private:
	Level* level { nullptr };
	ObjectPool* objectPool { nullptr };
	Player* player{ nullptr };

	EnemySystem* enemySystem{ nullptr };
};

