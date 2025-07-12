#pragma once
#include "levelTrigger.h"


class MetalGear;
class Elevator;

class ElevatorTrigger : public LevelTrigger
{
public:
	ElevatorTrigger() = delete;
	ElevatorTrigger(const AABB& aabb, Level& level, Player& player, bool crawlActivatable, Elevator* elevator, MetalGear* game);

	void ActivateTrigger() override;

private:
	Elevator* elevator{ nullptr };
	MetalGear* game{ nullptr };
};

