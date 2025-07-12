#pragma once

class Inventory;
class ResourceManager;
class Level;
class ObjectPool;
class Player;
class EnemySystem;
class InputManager;
class DialogueManager;
class MetalGear;
class MusicTrack;

enum class ScreenState
{
	Gameplay,
	GameplayPaused,
	Codec,
	Weapons,
	Equipment,
	SaveLoad

};

class ScreenManager: UIBase
{
public:
	ScreenManager() = default;
	void Init(const ResourceManager& resourceManager, Level* level, ObjectPool* objectpool, Player* player, EnemySystem* enemySystem, Inventory* inventory, DialogueManager* dialogueManager, MetalGear* game, irrklang::ISoundEngine* soundEngine);

	void Update(float deltaTime);
	void Draw(Surface& target, int2 offset = { 0,0 }) const;

	void SetScreen(ScreenState value);
	ScreenState GetScreenState() const;

private:
	TileSheet* pauseText{nullptr};

	skel::InputManager* inputManager{nullptr};

	ScreenState screenState{ ScreenState::Gameplay };

	GameScreen gameplayScreen;

	InventoryScreen weaponScreen;
	InventoryScreen equipmentScreen;

	CodecScreen codecScreen;

	SaveLoadScreen saveLoadScreen;


	irrklang::ISoundEngine* soundEngine{ nullptr };
};

