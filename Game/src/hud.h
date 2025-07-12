#pragma once
#include "Core/Timer.h"

class Level;
class ObjectPool;
class EnemySystem;
class ResourceManager;
class Inventory;
class Font;
class DialogueManager;
class TileSheet;

class HUD : public UIBase
{
public:
	HUD() = default;
	void Init(const ResourceManager& resourceManager, Level* level, Player* player, EnemySystem* enemySystem, Inventory* inventory, DialogueManager* dialogueManager, irrklang::ISoundEngine* soundEngine);

	void Update(float deltaTime) override;
	void Draw(Surface& target, int2 offset = { 0,0 }) const override;

private:
	EnemySystem* enemySystem{ nullptr };
	DialogueManager* dialogueManager{ nullptr };

	TileSheet* callText{ nullptr };
	Timer callTextTimer;
	bool showCallText{ false };

	// elements

	RadarUI radar;
	HealthBar healthBar;
	InventorySlots invenorySlots;

	// offsets
	
	int2 gameplayOffset = int2(8); // there will be a gap in the center that will be offset by this
	irrklang::ISoundEngine* soundEngine{ nullptr };
};

