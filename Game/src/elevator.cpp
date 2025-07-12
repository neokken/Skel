#include "precomp.h"

#include "resourceManager.h"

#include "tilesheet.h"
#include "tilemap.h"
#include "level.h"
#include "animatedSprite.h"

#include "Elevator.h"

Elevator::Elevator()
	: Entity(AABB({ 33.f,2.f }, { 41.f,12.f }), 1)
{
	sprite->AddAnimation(ELEVATOR_CLOSED, AnimationData(ELEVATOR_CLOSED_FRAME_COUNT, ELEVATOR_CLOSED_FRAMES, .1f));
	sprite->AddAnimation(ELEVATOR_OPENING, AnimationData(ELEVATOR_OPENING_FRAME_COUNT, ELEVATOR_OPENING_FRAMES, .1f));
	sprite->AddAnimation(ELEVATOR_OPEN, AnimationData(ELEVATOR_OPEN_FRAME_COUNT, ELEVATOR_OPEN_FRAMES, .1f));
}

void Elevator::Init(const ResourceManager& resourceManager, Level* _level, ObjectPool* _objectPool, irrklang::ISoundEngine* _soundEngine)
{
	Entity::Init(resourceManager.getTileSheet(TileSheets::Elevator), _level, _objectPool, _soundEngine);
	buttonLight = resourceManager.getTileSheet(TileSheets::ElevatorButton);
}

void Elevator::Update(float deltaTime)
{
	
	switch (state)
	{
	case ElevatorState::Closed:
		sprite->SetAnimation(ELEVATOR_CLOSED);
		break;
	case ElevatorState::Opening:
		delay -= deltaTime;
		if (delay <= 0.f)
		{
			sprite->SetAnimation(ELEVATOR_OPENING);
		}

		if (sprite->GetFrameIndex() == ELEVATOR_OPENING_FRAME_COUNT-1)
		{
			state = ElevatorState::Open;
			sprite->SetAnimation(ELEVATOR_OPEN);
		}
		break;
	case ElevatorState::Open:
		sprite->SetAnimation(ELEVATOR_OPEN);
		break;
	default:
		break;
	}

	sprite->Update(deltaTime);
}

void Elevator::Draw(Surface& target, int2 offset) const
{
	Entity::Draw(target, offset);
	if (state != ElevatorState::Closed)
	{
		buttonLight->DrawTile(target, level->worldToScreenSpace(GetPosition()) + int2(35,4) + offset, 0);
	}
}

void Elevator::DrawDebug(Surface& target, int2 offset) const
{

	GetCollider().draw(target, level->GetWorldOffset() + offset, 0xffffffff);
}

void Elevator::OnDeath()
{
	if (state == ElevatorState::Closed)
	{
		state = ElevatorState::Opening;
	}
}

ElevatorState Elevator::GetElevatorState() const
{
	return state;
}

