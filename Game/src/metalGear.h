#pragma once



struct GameData
{
	//
	int2 playerPosition; // gonna clamp this to a int2
	int2 currentRoom;
	int2 pickupableLocations[8]; // all the locations of pickupables, So if you picked it up it is removed from this list and so going to remove them when loading the save. 8 is now the maxamount of pickupables can be more but I dont think we ever have more than 8 pickupables in a level
	bool isCrawling;
	Direction lookdir;

	int maxHealth;
	int currentHealth;

	// level
	int currentLevel;
	

	// inventorystuff;
	int ammoCount;
	ItemType equipment[INVENTORY_SLOT_COUNT];
	ItemType weapons[INVENTORY_SLOT_COUNT];

	//levelpickupables


};

class ResourceManager;

class Inventory;
class Player;

class MusicTrack;

enum class MenuState
{
	Game,
	MainMenu,
	DeathMenu,
	VictoryMenu,
};


class MetalGear
{
public:
	MetalGear();
	~MetalGear();
	void Init(ResourceManager* resourceManager, irrklang::ISoundEngine* soundEngine);


	void Update(float deltaTime);

	void Draw(Surface& target) const;

	// gameFunctions 

	void LoadLevel(int level);

	void LoadLevel(int levelIndex, int2 roomPosition, float2 teleportPosition);

	Player* GetPlayer() const;

	// loading and saving gameData

	void LoadGameData(const GameData& gameData);

	GameData GetGameData() const;

	void ReadGameData();
	void WriteGameData(const GameData& gameData);

	void SetDead();
	void SetVictory();


	bool IsRunning() const;

private:
	void LoadLevel0();
	void LoadLevel1();


	void UpdateMainMenu(float deltaTime);
	void DrawMainMenu(Surface& target) const;

	void UpdateDeathMenu(float deltaTime);
	void DrawDeathMenu(Surface& target) const;

	void UpdateVictoryMenu(float deltaTime);
	void DrawVictoryMenu(Surface& target) const;

	int menuOption{ 0 };
	TileSheet* mainMenuTexture{ nullptr };
	TextBox textbox{};


	ResourceManager* resourceManager{ nullptr }; //non owning

	MenuState menuState{ MenuState::MainMenu };

	// maybe should be pointers?
	// so we don't have to include everything where we use this
 	DialogueManager dialogueManager;

	ObjectPool objectPool;
	EnemySystem enemySystem;

	Level level;
	int currentLevel{ 0 };

	Inventory* inventory{ nullptr };
	Player* player{ nullptr };


	HUD gameHUD;
	ScreenManager gameScreens;

	irrklang::ISoundEngine* soundEngine{ nullptr };
	MusicTrack* music;
	MusicTrack* codecCall;

	bool exitGame{ false };
};

