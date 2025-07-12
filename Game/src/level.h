#pragma once

class TileSheet;
class Player;

struct AABB;
class LevelTrigger;
class DialogueManager;
class Elevator;

class MetalGear;

enum class CollisionTileType
{
	WALKABLE_OPEN		= 0,	// Open, walkable
	FULL_COLLISION		= 1,	// Full collision, neither walk nor crawl
	CRAWL_ONLY			= 2,	// Crawl only
	CRAWL_ONLY_OVERLAY	= 3,	// Crawl only but if you are forced to crawl you draw on top of it
	CRAWL_COLLISION 	= 4,	// only blocking when you are forced to crawl
	WALK_HALF_OVERLAY	= 5,	// Walkable, but when crawling draw half the tile on top of the player
	WALKABLE_CLOSED		= 6,	// Walkable, but radar sees it as closed
	FORCED_CRAWL_OVERLAY= 7,	// Walkable, but draw over if forced to crawl
	CRAWL_OVERLAY_NO_FORCE = 8,	// Walkable, but draw over if forced to crawl
	UNKNOWN
};



class Level
{
public:
	Level() = default;
	Level(const char* levelVisualMapPath, const char* levelCollisionMapPath, uint levelWidth, uint levelHeight);
	

	//Level(const Level& other); no copying because of inheritance pointers (will look into this later)

	Level(const Level& other);
	Level(Level&& other) noexcept;

	//Level& operator=(const Level& other) ;
	Level& operator=(Level&& other) noexcept;

	~Level();

	void Init(TileSheet* visual, TileSheet* collision, DialogueManager* dialogueManager);


	void Update(Player& player);

	// == Drawing == 
	
	// Can be optimized since we don't have to draw the whole map, we can just base it off the world offset to draw start,
	// and end length will be the same 24x24
	void DrawVisual(Surface& target, int2 offset={0,0});
	void DrawTile(Surface& target, int2 tilePositon, int2 offset = { 0,0 });
	void DrawTopHalfTile(Surface& target, int2 tilePositon, int2 offset = { 0,0 });

	void DrawDebug(Surface& target, int2 offset={0,0});

	void DrawTriggers(Surface& target, int2 offset = { 0,0 });

	// == Physics ==

	void ResolvePlayerCollision(Player& player) const;


	bool RaycastCollision(const float2& point1, const float2& point2, float2& hitPoint)  const;

	// == Position ==

	int2 worldToScreenSpace(int2 position) const;
	int2 worldToScreenSpace(float2 position) const;
	
	float2 screenToWorldSpace(int2 position) const;
	float2 screenToWorldSpace(float2 position) const;

	// == Getters & Setters ==
	bool IsWalkable(CollisionTileType collider) const;


	const uint2& GetLevelSize() const;

	const int2& GetActiveRoom() const;
	const int2& GetWorldOffset() const;

	CollisionTileType GetColliderTile(int2 tilePosition) const;


	void SetActiveRoom(const int2& room);
	

	void AddRoomTeleportTrigger(const AABB& collider, Player& player, float2 playerPosition, bool crawlActivatable=true, bool forceUncrawl=true);
	void AddLevelTeleportTrigger(const AABB& collider, Player& player, MetalGear* game, int levelIndex, float2 playerPosition, bool forceUnCrawl = false, bool crawlActivatable = true);

	void AddDialogueTrigger(const AABB& collider, Player& player, const CodecDialogue* dialogue, bool crawlActivatable = true);
	
	void AddCrawlTrigger(const AABB& collider, Player& player, bool vertical, float minRange, float maxRange);

	void AddElevatorTrigger(const AABB& collider, Player& player, Elevator* elevator, MetalGear* game, bool crawlActivatable = true);

private:
	uint2 levelSize{ 0 };	// amount of tiles horizontal and vertical same as the tilemapSize so can be removed and go through tilemap
	
	int2 activeRoom{ 0 };
	int2 worldOffset{ 0,0 };

	TileMap visualLayer;
	TileMap collisionLayer;

	ArrayList<CollisionTileType> walkableColliders;

	LevelTrigger* levelTriggers[MAX_TRIGGERS_AMOUNT]{ nullptr };
	uint levelTriggerCount{ 0 };

	DialogueManager* dialogueManager{ nullptr };
};
