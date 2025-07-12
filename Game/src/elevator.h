#pragma once
#include "aabb.h" // I am sorry. But not really I defend that for inhertance it does make sense to include in header.
#include "gameobject.h"
#include "entity.h"


enum class ElevatorState
{
	Closed,
	Opening,
	Open,
};

class Elevator : public Entity
{
public:
	Elevator();
	~Elevator() = default;

	void Init(const ResourceManager& resourceManager, Level* _level, ObjectPool* _objectPool, irrklang::ISoundEngine* soundEngine);
	
	void Update(float deltaTime) override;

	void Draw(Surface& target, int2 offset) const override;
	void DrawDebug(Surface& target, int2 offset) const override;

	void OnDeath() override;

	ElevatorState GetElevatorState() const;
private:
	ElevatorState state{ ElevatorState::Closed };
	TileSheet* buttonLight{ nullptr };

	float delay = 5.f;

};

