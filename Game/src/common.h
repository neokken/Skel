// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

// common.h is to be included in host and device code and stores
// global settings and defines.



// DEBUG STUFF

//#define TERRAIN_DEBUG
//#define TRIGGER_DEBUG
//#define OBJECT_DEBUG	// needs to be on for player or enemy debug
//#define PLAYER_DEBUG
//#define ENEMY_DEBUG
//#define BULLET_DEBUG
//#define PICKUPABLE_DEBUG
//#define ELEVATOR_DEBUG


//#define SPECIAL_PORTRAITS // the *real* portraits on the codec

//#define CLICK_POSITION_DEBUG // click on the screen and look in the console

//#define PLAYER_POSITION_DEBUG // press B for player position

//#define UNSEEABLE
//#define NOCLIP

// default screen resolution


#include <string>
constexpr int GAME_WINDOW_WIDTH = 288; // 288  (24*8=192)
constexpr int GAME_WINDOW_HEIGHT = 220; // 216 (24*8=192)

constexpr uint WINDOW_SCALE_FACTOR = 4;

#define SCRWIDTH	GAME_WINDOW_WIDTH*WINDOW_SCALE_FACTOR
#define SCRHEIGHT	GAME_WINDOW_HEIGHT*WINDOW_SCALE_FACTOR

constexpr uint DEFAULT_MASK_COLOR = 0xff7f5b00;



// #define FULLSCREEN

// constants
constexpr float PI = 3.14159265358979323846264f;
constexpr float INVPI = 0.31830988618379067153777f;
constexpr float INV2PI = 0.15915494309189533576888f;
constexpr float TWOPI = 6.28318530717958647692528f;
constexpr float SQRT_PI_INV = 0.56418958355f;
constexpr float LARGE_FLOAT = 1e34f;



// RESOURCE MANAGER

enum class TileSheets
{
	Empty,
	TerrainCollision,
	OutsideTerrain,
	InsideTerrain,

	Snake,
	Enemy,
	EnemyIcon,
	Bullet,

	Items,
	
	Elevator,
	ElevatorButton,
	// HUD
	MainMenuTexture,

	Radar,
	RadarIndicator,
	RadarDecoration,
	
	ItemSlot,

	HealthBar,

	PauseText,
	CallText,

	// CODEC

	Codec,
	CodecRecvSend,
	CodecConnection,

	Portraits,
	PortraitBorder,


	Count
};

enum class Fonts
{
	MainFont,
	RadarNumbers,

	ItemAmountNumbers,
	CodecFrequency,

	Count
};


static uint seed = 0x12345678;

// WangHash: calculates a high-quality seed based on an arbitrary non-zero
// integer. Use this to create your own seed based on e.g. thread index.
static uint WangHash(uint s)
{
	s = (s ^ 61) ^ (s >> 16);
	s *= 9, s = s ^ (s >> 4);
	s *= 0x27d4eb2d;
	s = s ^ (s >> 15);
	return s;
}
static uint InitSeed(uint seedBase)
{
	return WangHash((seedBase + 1) * 17);
}

// RandomUInt()
// Update the seed and return it as a random 32-bit unsigned int.
static uint RandomUInt()
{
	seed ^= seed << 13;
	seed ^= seed >> 17;
	seed ^= seed << 5;
	return seed;
}

// RandomFloat()
// Calculate a random unsigned int and cast it to a float in the range
// [0..1)
static float RandomFloat() { return RandomUInt() * 2.3283064365387e-10f; }
static float Rand(float range) { return RandomFloat() * range; }

// Calculate a random number based on a specific seed
static uint RandomUInt(uint& customSeed)
{
	customSeed ^= customSeed << 13;
	customSeed ^= customSeed >> 17;
	customSeed ^= customSeed << 5;
	return customSeed;
}
static float RandomFloat(uint& customSeed) { return RandomUInt(customSeed) * 2.3283064365387e-10f; }



struct TileSheetData
{
	TileSheetData(const char* p, uint2 cell)
		: path{p}
		  , cellSize{ cell } {}

	const char* path{ nullptr };
	uint2 cellSize;

};


struct FontData
{
	FontData(const char* path, uint2 charSize, uint ASCIIStart, uint ASCIIEnd)
		: path{ path}
		, cellSize{charSize}
		, asciiStart{ ASCIIStart }
		, asciiEnd{ ASCIIEnd } {}

	const char* path;
	uint2 cellSize;
	uint asciiStart;
	uint asciiEnd;
};


static const TileSheetData tileSheetData[] {
	{ "assets/Empty.png", uint2(16, 16) },
	{ "assets/collisionTileSheet.png", uint2(8, 8) },
	{ "assets/outsideTileSheet.png", uint2(8, 8) },
	{ "assets/insideTileSheet.png", uint2(8, 8) },
	  
	{ "assets/snake.png", uint2(32,32)},
	{ "assets/enemies.png", uint2(16, 32)},
	{ "assets/EnemyIcon.png", uint2(16, 16)},
	{ "assets/bullet.png", uint2(2,2)},

	{ "assets/ItemSprites.png",uint2(16, 16)},

	{ "assets/Elevator.png",uint2(33, 26)},
	{ "assets/ElevatorButton.png",uint2(5, 7)},

	{ "assets/MainMenu.png", uint2(263, 128) },

	{ "assets/UI/radar.png", uint2(58, 58) },
	{ "assets/UI/radarIndicator.png", uint2(24, 24) },
	{ "assets/UI/radarDecoration.png", uint2(56, 14) },

	{ "assets/UI/ItemSlot.png", uint2(48, 36) },
	
	{ "assets/UI/healthBar.png", uint2(20, 12) },
	{ "assets/UI/Pause.png", uint2(38, 15) },

	{"assets/UI/CallText.png", uint2(32, 10)},

	{ "assets/Codec/Codec.png", uint2(60, 146) },
	{ "assets/Codec/RecvSend.png", uint2(22, 7) },

	{ "assets/Codec/Connection.png", uint2(42, 12) },


	{ "assets/Codec/CodecPortraits.png", uint2(32, 48) },

	{ "assets/Codec/PortaitBorder.png", uint2(36, 52) },
	
	
};


static const FontData fontData[] {
	{ "assets/mainFont.png", uint2(8), 33u, 126u },
	{ "assets/RadarFont.png", uint2(5), 48u, 57u },

	{ "assets/ItemCountFont.png", uint2(6,13), 48u, 57u },
	{ "assets/FrequencyFont.png", uint2(6,14), 48u, 57u },
};




enum class Characters
{
	None,
	Snake,
	SnakeCigar,
	Campbell,
	Holly,
};


static const char* CharacterNames[]{
	"None",
	"SNAKE",
	"SNAKE",
	"CAMPB",
	"HOLLY",
};

//		======= ITEM DATA =======

enum class ItemType
{
	Cigar,
	Ration,
	Ammo,
	Pistol,
	Empty,
};


constexpr uint ITEM_TILEINDEX[] = { 0 ,1 ,2 ,3, 5 };

constexpr const char* ITEM_NAMES[] = { 
	"Cigar", 
	"Ration", 
	"Ammo", 
	"Pistol", 
	"Empty" };

constexpr const char* ITEM_DESCRIPTIONS[] = { 
	"Cigarettes\n", 

	"Ration\n\
I'm getting hungry.", 
	
	"Ammo\n\
How tf did you get here?\
Hacker man >:O", 
	
	"Pistol\n\
PEW PEW!! Crazy guy \n\
over here.", 
	
	"Empty" };


// codec data

struct CodecLine
{
	CodecLine() = default;
	CodecLine(bool isSending, const char* dialogue)
		: isSending{isSending}
		, dialogueText{dialogue} {}

	bool isSending{false};
	const char* dialogueText{nullptr};
};


struct CodecDialogue
{
	CodecDialogue(Characters left, bool _leftGlitch, Characters right, bool _rightGlitch, int _frequency, const CodecLine* dialogue, uint dialogueCount)
		: leftCharacter{ left }
		, rightCharacter{ right }
		, leftGlitching{ _leftGlitch }
		, rightGlitching{ _rightGlitch }
		, frequency{ _frequency }
		, dialogueLines{dialogue}
		, dialogueLineCount{ dialogueCount } {}

	Characters leftCharacter{ Characters::Snake };
	Characters rightCharacter{ Characters::None };

	bool leftGlitching{ false };
	bool rightGlitching{ false };

	int frequency{ 0 };
	const CodecLine* dialogueLines{nullptr};
	uint dialogueLineCount{ 0 };
};


// DIALOGUE

constexpr float TIME_PER_CHARACTER = .1f;

static const CodecLine INTRO_DIALOGUE_TEXT[] = { 
	CodecLine(true, "This is snake...\nArrived at\nentrace point."),
	CodecLine(false, "Snake...\nRight on time,\nas usual."),
	CodecLine(false, "OK...\nLet's begin\nOperation\nIntrude F014."),
	CodecLine(false, "Remember, your\nmission is to\nsneak into\nZanzibar Land."),
	CodecLine(false, "Then you retake\nkidnapped Czech\nGame Programmer\nSeb."),
	CodecLine(false, "Now turn on your\nmobile radar."),
	CodecLine(true, "OK...\nSwitched on."),
	CodecLine(false, "This radar\nis equipped\nwith various\nsensors."),
	CodecLine(false, "It will warn\nyou of many\ndangers."),
	CodecLine(true, "What's its range?"),
	CodecLine(false, "9 screens, with\nyou in the\nmiddle."),
	CodecLine(false, "The radar\ndeactivates\nwhen an enemy\nfinds you,"),
	CodecLine(false, "because the\nenemy can\ndetect the\nradar signal."),
	CodecLine(true, "I see.\n...Where is\nthis Seb?"),
	CodecLine(false, "Seb\nhas an emitter\nin his tooth."),
	CodecLine(false, "If you come near\nhim, he'll show\nup on your radar\nas a red dot."),
	CodecLine(true, "Let's find that\nred dot then..."),
	CodecLine(false, "Snake...\nCommunications\nwill be on freq.\n140.85."),
	CodecLine(false, "Good luck!\n...Over"),
};

static const CodecLine CAMPB_FREQ[] = {
	CodecLine(true, "This is solid\nSnake. Your\nreply please..."),
	CodecLine(false, "Fight when you\nshould fight.\nDestoy when you\nshould destroy"),
	CodecLine(false, "Basic rules on\n battlefields and\nin shoot-em-ups.\n...Over"),
};

static const CodecLine CAMPB_FREQ2[] = {
	CodecLine(true, "This is solid\nSnake. Your\nreply please..."),
	CodecLine(false, "If you are in\ntrouble, contact\nMcdonnel Miller."),
	CodecLine(false, "He is our former\nsurvival master.\nHe has a lot of\nuseful ideas."),
	CodecLine(false, "His frequency is\n 140.38.\n...Over"),
};


static const CodecLine FENCE_DIALOGUE_TEXT[] = {
	CodecLine(false, "Crawl and sneak\nthrough the gap\nin the fence.\n...Over"),
};

static const CodecLine ENTRRANCE_DIALOGUE_TEXT[] = {
	CodecLine(false, "Sneaking in from\nthe front is\nimpossible."),
	CodecLine(false, "Look for an\nalternative\nway in.\n...Over"),
};

static const CodecLine HOLLY_DIALOGUE_TEXT[] = {
	CodecLine(false, "I'm Holly,\nHolly White."),
	CodecLine(false, "I entered\nZanzibar Land\na month ago as\na journalist."),
	CodecLine(false, "I think I can be\nof assistance."),
	CodecLine(false, "My frequency is\n140.15. Will you\ncall me?\n...Bye"),
};


static const CodecDialogue INTRO_DIALOGUE = CodecDialogue(Characters::Snake, false, Characters::Campbell, true, 85, INTRO_DIALOGUE_TEXT, 19);

static const CodecDialogue FENCE_DIALOGUE = CodecDialogue(Characters::Snake, false, Characters::Campbell, true, 85, FENCE_DIALOGUE_TEXT, 1);
static const CodecDialogue ENTRANCE_DIALOGUE = CodecDialogue(Characters::Snake, false, Characters::Campbell, true, 85, ENTRRANCE_DIALOGUE_TEXT, 2);

static const CodecDialogue HOLLY_DIALOGUE = CodecDialogue(Characters::Snake, false, Characters::Holly, true, 15, HOLLY_DIALOGUE_TEXT, 4);

static const CodecDialogue CAMPB0_DIALOGUE = CodecDialogue(Characters::Snake, false, Characters::Campbell, false, 85, CAMPB_FREQ, 3);
static const CodecDialogue CAMPB1_DIALOGUE = CodecDialogue(Characters::Snake, false, Characters::Campbell, false, 85, CAMPB_FREQ2, 4);




// EMPTY_FREQUENCY

static const CodecLine EMPTY_FREQUENCY_TEXT[] = { CodecLine(true, "Hello this is snake!")};
static const CodecDialogue EMPTY_FREQUENCY_DIALOGUE = CodecDialogue(Characters::Snake, false, Characters::None, false, 11, EMPTY_FREQUENCY_TEXT, 1);



//		======= ROOM DATA =======


constexpr uint ROOM_WIDTH = 24;
constexpr uint ROOM_HEIGHT = 24;


constexpr uint TERRAIN_TILE_SIZE = 8;




//		======= LEVEL =======


// reading the csv room we need to read ints as chars so we need to allocate some space for the values
// 3 means 3 base-ten characters so to max: 999, 4 would be 9999 etc..
constexpr int TILEINDEXCHARACTERAMOUNT = 4;


constexpr char OUTSIDE_LEVEL_VISUAL_PATH[] = "assets/LevelData/outside_Visuals.csv";
constexpr char OUTSIDE_LEVEL_COLLISION_PATH[] = "assets/LevelData/outside_Collisions.csv";


constexpr char INSIDE_LEVEL_VISUAL_PATH[] = "assets/LevelData/inside_Visuals.csv";
constexpr char INSIDE_LEVEL_COLLISION_PATH[] = "assets/LevelData/inside_Collisions.csv";

constexpr uint MAX_TRIGGERS_AMOUNT = 32;
constexpr uint MAX_PATROLLING_ENEMIES = 16;


//		======= BULLET DATA =======

constexpr float BULLET_SPEED = 60.f;
constexpr int BULLET_DAMAGE = 6;

constexpr int BULLET_TERRAIN_HITBOX_OFFSET = 17;
constexpr float BULLET_MAX_LIFETIME = 10.f;



//		======= ENEMY DATA =======

constexpr int ENEMY_MAX_HEALTH = 6;

constexpr float ENEMY_SHOOT_DELAY = 2.f;
constexpr float ENEMY_SHOOT_DISTANCE = static_cast<float>(TERRAIN_TILE_SIZE * 8);
constexpr float ENEMY_DAMAGE_STUN_DURATION = 2.5f;

constexpr float ENEMY_CONFUSE_TIME = 6.f;

//		======= ENEMY SYSTEM DATA =======


constexpr float EVADE_TIME_LENGTH = 15.f;

constexpr float ALARM_TIME_LENGTH = 5.f;

constexpr uint MAX_SEARCHING_ENEMIES = 4;

constexpr float ENEMY_SPAWN_DELAY = 1.f;



//		======= PLAYER DATA =======

constexpr int INVENTORY_SLOT_COUNT = 14;
 
constexpr int SNAKE_MAX_HEALTH = 32;
constexpr int SNAKE_PUNCH_DAMAGE = 2;

constexpr float SNAKE_PUNCH_COOLDOWN = .6f;
constexpr float SNAKE_PUNCH_FRAME_TIME = .3f;

const float2 SNAKE_WALK_HITBOX_OFFSET(8.f, 24.f);
const float2 SNAKE_WALK_HITBOX_SIZE(15.f, 7.f);

const float2 SNAKE_CRAWLH_HITBOX_OFFSET(2.f, 25.f);
const float2 SNAKE_CRAWLH_HITBOX_SIZE(28.f, 5.f);

const float2 SNAKE_CRAWLV_HITBOX_OFFSET(9.f, 4.f);
const float2 SNAKE_CRAWLV_HITBOX_SIZE(13.f, 28.f);



//		=========== ANIMATIONS DATA ===========


constexpr uint MAX_ANIMATIONS = 48;
constexpr uint MAX_ANIMATION_FRAMES = 16;

constexpr float TIME_PER_FRAME_ANIMATION = .15f;



//		======= SNAKE ANIMATIONDATA =======

constexpr uint SNAKE_IDLE_UP = 0;
constexpr uint SNAKE_IDLE_UP_FRAME_COUNT = 1;
constexpr uint SNAKE_IDLE_UP_FRAMES[SNAKE_IDLE_UP_FRAME_COUNT] = { 1 };

constexpr uint SNAKE_IDLE_DOWN = 1;
constexpr uint SNAKE_IDLE_DOWN_FRAME_COUNT = 1;
constexpr uint SNAKE_IDLE_DOWN_FRAMES[SNAKE_IDLE_DOWN_FRAME_COUNT] = { 12 };

constexpr uint SNAKE_IDLE_LEFT = 2;
constexpr uint SNAKE_IDLE_LEFT_FRAME_COUNT = 1;
constexpr uint SNAKE_IDLE_LEFT_FRAMES[SNAKE_IDLE_LEFT_FRAME_COUNT] = { 23 };

constexpr uint SNAKE_IDLE_RIGHT = 3;
constexpr uint SNAKE_IDLE_RIGHT_FRAME_COUNT = 1;
constexpr uint SNAKE_IDLE_RIGHT_FRAMES[SNAKE_IDLE_RIGHT_FRAME_COUNT] = { 34 };


constexpr uint SNAKE_WALK_UP = 4;
constexpr uint SNAKE_WALK_UP_FRAME_COUNT = 4;
constexpr uint SNAKE_WALK_UP_FRAMES[SNAKE_WALK_UP_FRAME_COUNT] = { 0, 1, 2, 1 };

constexpr uint SNAKE_WALK_DOWN = 5;
constexpr uint SNAKE_WALK_DOWN_FRAME_COUNT = 4;
constexpr uint SNAKE_WALK_DOWN_FRAMES[SNAKE_WALK_DOWN_FRAME_COUNT] = { 11, 12, 13, 12 };

constexpr uint SNAKE_WALK_LEFT = 6;
constexpr uint SNAKE_WALK_LEFT_FRAME_COUNT = 4;
constexpr uint SNAKE_WALK_LEFT_FRAMES[SNAKE_WALK_LEFT_FRAME_COUNT] = { 22, 23, 24, 23 };

constexpr uint SNAKE_WALK_RIGHT = 7;
constexpr uint SNAKE_WALK_RIGHT_FRAME_COUNT = 4;
constexpr uint SNAKE_WALK_RIGHT_FRAMES[SNAKE_WALK_RIGHT_FRAME_COUNT] = { 33, 34, 35, 34 };


constexpr uint SNAKE_PUNCH_UP = 8;
constexpr uint SNAKE_PUNCH_UP_FRAME_COUNT = 1;
constexpr uint SNAKE_PUNCH_UP_FRAMES[SNAKE_PUNCH_UP_FRAME_COUNT] = { 6 };

constexpr uint SNAKE_PUNCH_DOWN = 9;
constexpr uint SNAKE_PUNCH_DOWN_FRAME_COUNT = 1;
constexpr uint SNAKE_PUNCH_DOWN_FRAMES[SNAKE_PUNCH_DOWN_FRAME_COUNT] = { 17 };

constexpr uint SNAKE_PUNCH_LEFT = 10;
constexpr uint SNAKE_PUNCH_LEFT_FRAME_COUNT = 1;
constexpr uint SNAKE_PUNCH_LEFT_FRAMES[SNAKE_PUNCH_LEFT_FRAME_COUNT] = { 28 };

constexpr uint SNAKE_PUNCH_RIGHT = 11;
constexpr uint SNAKE_PUNCH_RIGHT_FRAME_COUNT = 1;
constexpr uint SNAKE_PUNCH_RIGHT_FRAMES[SNAKE_PUNCH_RIGHT_FRAME_COUNT] = { 39 };



constexpr uint SNAKE_KNEEL_UP = 12;
constexpr uint SNAKE_KNEEL_UP_FRAME_COUNT = 1;
constexpr uint SNAKE_KNEEL_UP_FRAMES[SNAKE_KNEEL_UP_FRAME_COUNT] = { 7 };

constexpr uint SNAKE_KNEEL_DOWN = 13;
constexpr uint SNAKE_KNEEL_DOWN_FRAME_COUNT = 1;
constexpr uint SNAKE_KNEEL_DOWN_FRAMES[SNAKE_KNEEL_DOWN_FRAME_COUNT] = { 18 };

constexpr uint SNAKE_KNEEL_LEFT = 14;
constexpr uint SNAKE_KNEEL_LEFT_FRAME_COUNT = 1;
constexpr uint SNAKE_KNEEL_LEFT_FRAMES[SNAKE_KNEEL_LEFT_FRAME_COUNT] = { 29 };

constexpr uint SNAKE_KNEEL_RIGHT = 15;
constexpr uint SNAKE_KNEEL_RIGHT_FRAME_COUNT = 1;
constexpr uint SNAKE_KNEEL_RIGHT_FRAMES[SNAKE_KNEEL_RIGHT_FRAME_COUNT] = { 40 };



constexpr uint SNAKE_KNEEL_WALK_UP = 16;
constexpr uint SNAKE_KNEEL_WALK_UP_FRAME_COUNT = 1;
constexpr uint SNAKE_KNEEL_WALK_UP_FRAMES[SNAKE_KNEEL_WALK_UP_FRAME_COUNT] = { 7 };

constexpr uint SNAKE_KNEEL_WALK_DOWN = 17;
constexpr uint SNAKE_KNEEL_WALK_DOWN_FRAME_COUNT = 1;
constexpr uint SNAKE_KNEEL_WALK_DOWN_FRAMES[SNAKE_KNEEL_WALK_DOWN_FRAME_COUNT] = { 18 };

constexpr uint SNAKE_KNEEL_WALK_LEFT = 18;
constexpr uint SNAKE_KNEEL_WALK_LEFT_FRAME_COUNT = 1;
constexpr uint SNAKE_KNEEL_WALK_LEFT_FRAMES[SNAKE_KNEEL_WALK_LEFT_FRAME_COUNT] = { 29 };

constexpr uint SNAKE_KNEEL_WALK_RIGHT = 19;
constexpr uint SNAKE_KNEEL_WALK_RIGHT_FRAME_COUNT = 1;
constexpr uint SNAKE_KNEEL_WALK_RIGHT_FRAMES[SNAKE_KNEEL_WALK_RIGHT_FRAME_COUNT] = { 40 };



constexpr uint SNAKE_CRAWL_IDLE_UP = 20;
constexpr uint SNAKE_CRAWL_IDLE_UP_FRAME_COUNT = 1;
constexpr uint SNAKE_CRAWL_IDLE_UP_FRAMES[SNAKE_CRAWL_IDLE_UP_FRAME_COUNT] = { 9 };

constexpr uint SNAKE_CRAWL_IDLE_DOWN = 21;
constexpr uint SNAKE_CRAWL_IDLE_DOWN_FRAME_COUNT = 1;
constexpr uint SNAKE_CRAWL_IDLE_DOWN_FRAMES[SNAKE_CRAWL_IDLE_DOWN_FRAME_COUNT] = { 20 };

constexpr uint SNAKE_CRAWL_IDLE_LEFT = 22;
constexpr uint SNAKE_CRAWL_IDLE_LEFT_FRAME_COUNT = 1;
constexpr uint SNAKE_CRAWL_IDLE_LEFT_FRAMES[SNAKE_CRAWL_IDLE_LEFT_FRAME_COUNT] = { 31 };

constexpr uint SNAKE_CRAWL_IDLE_RIGHT = 23;
constexpr uint SNAKE_CRAWL_IDLE_RIGHT_FRAME_COUNT = 1;
constexpr uint SNAKE_CRAWL_IDLE_RIGHT_FRAMES[SNAKE_CRAWL_IDLE_RIGHT_FRAME_COUNT] = { 42 };


constexpr uint SNAKE_CRAWL_UP = 24;
constexpr uint SNAKE_CRAWL_UP_FRAME_COUNT = 3;
constexpr uint SNAKE_CRAWL_UP_FRAMES[SNAKE_CRAWL_UP_FRAME_COUNT] = { 8,9,10 };

constexpr uint SNAKE_CRAWL_DOWN = 25;
constexpr uint SNAKE_CRAWL_DOWN_FRAME_COUNT = 3;
constexpr uint SNAKE_CRAWL_DOWN_FRAMES[SNAKE_CRAWL_DOWN_FRAME_COUNT] = { 19,20,21 };

constexpr uint SNAKE_CRAWL_LEFT = 26;
constexpr uint SNAKE_CRAWL_LEFT_FRAME_COUNT = 3;
constexpr uint SNAKE_CRAWL_LEFT_FRAMES[SNAKE_CRAWL_LEFT_FRAME_COUNT] = { 30,31,32 };

constexpr uint SNAKE_CRAWL_RIGHT = 27;
constexpr uint SNAKE_CRAWL_RIGHT_FRAME_COUNT = 3;
constexpr uint SNAKE_CRAWL_RIGHT_FRAMES[SNAKE_CRAWL_RIGHT_FRAME_COUNT] = { 41,42,43 };


constexpr uint SNAKE_GUN_IDLE_UP = 28;
constexpr uint SNAKE_GUN_IDLE_UP_FRAME_COUNT = 1;
constexpr uint SNAKE_GUN_IDLE_UP_FRAMES[SNAKE_GUN_IDLE_UP_FRAME_COUNT] = { 4 };
					
constexpr uint SNAKE_GUN_IDLE_DOWN = 29;
constexpr uint SNAKE_GUN_IDLE_DOWN_FRAME_COUNT = 1;
constexpr uint SNAKE_GUN_IDLE_DOWN_FRAMES[SNAKE_GUN_IDLE_DOWN_FRAME_COUNT] = { 15 };
					
constexpr uint SNAKE_GUN_IDLE_LEFT = 30;
constexpr uint SNAKE_GUN_IDLE_LEFT_FRAME_COUNT = 1;
constexpr uint SNAKE_GUN_IDLE_LEFT_FRAMES[SNAKE_GUN_IDLE_LEFT_FRAME_COUNT] = { 26 };
					
constexpr uint SNAKE_GUN_IDLE_RIGHT = 31;
constexpr uint SNAKE_GUN_IDLE_RIGHT_FRAME_COUNT = 1;
constexpr uint SNAKE_GUN_IDLE_RIGHT_FRAMES[SNAKE_GUN_IDLE_RIGHT_FRAME_COUNT] = { 37 };
					
					
constexpr uint SNAKE_GUN_WALK_UP = 32;
constexpr uint SNAKE_GUN_WALK_UP_FRAME_COUNT = 4;
constexpr uint SNAKE_GUN_WALK_UP_FRAMES[SNAKE_GUN_WALK_UP_FRAME_COUNT] = { 3, 4, 5, 4 };
					
constexpr uint SNAKE_GUN_WALK_DOWN = 33;
constexpr uint SNAKE_GUN_WALK_DOWN_FRAME_COUNT = 4;
constexpr uint SNAKE_GUN_WALK_DOWN_FRAMES[SNAKE_GUN_WALK_DOWN_FRAME_COUNT] = { 14, 15, 16, 15 };
					
constexpr uint SNAKE_GUN_WALK_LEFT = 34;
constexpr uint SNAKE_GUN_WALK_LEFT_FRAME_COUNT = 4;
constexpr uint SNAKE_GUN_WALK_LEFT_FRAMES[SNAKE_GUN_WALK_LEFT_FRAME_COUNT] = { 25, 26, 27, 26 };
					
constexpr uint SNAKE_GUN_WALK_RIGHT = 35;
constexpr uint SNAKE_GUN_WALK_RIGHT_FRAME_COUNT = 4;
constexpr uint SNAKE_GUN_WALK_RIGHT_FRAMES[SNAKE_GUN_WALK_RIGHT_FRAME_COUNT] = { 36, 37, 38, 37 };


constexpr uint SNAKE_DEATH = 36;
constexpr uint SNAKE_DEATH_FRAME_COUNT = 15;
constexpr uint SNAKE_DEATH_FRAMES[SNAKE_DEATH_FRAME_COUNT] = { 1, 12, 23, 34, 1, 12, 23, 34, 7, 18, 29, 40,9 ,9,9};


//		======= ENEMY ANIMATIONDATA =======

constexpr uint ENEMY_IDLE_DOWN = 0;
constexpr uint ENEMY_IDLE_DOWN_FRAME_COUNT = 1;
constexpr uint ENEMY_IDLE_DOWN_FRAMES[ENEMY_IDLE_DOWN_FRAME_COUNT] = { 0 };

constexpr uint ENEMY_IDLE_LEFT = 1;
constexpr uint ENEMY_IDLE_LEFT_FRAME_COUNT = 1;
constexpr uint ENEMY_IDLE_LEFT_FRAMES[ENEMY_IDLE_LEFT_FRAME_COUNT] = { 4 };

constexpr uint ENEMY_IDLE_UP = 2;
constexpr uint ENEMY_IDLE_UP_FRAME_COUNT = 1;
constexpr uint ENEMY_IDLE_UP_FRAMES[ENEMY_IDLE_UP_FRAME_COUNT] = { 8 };

constexpr uint ENEMY_IDLE_RIGHT = 3;
constexpr uint ENEMY_IDLE_RIGHT_FRAME_COUNT = 1;
constexpr uint ENEMY_IDLE_RIGHT_FRAMES[ENEMY_IDLE_RIGHT_FRAME_COUNT] = { 12 };


constexpr uint ENEMY_WALK_DOWN = 4;
constexpr uint ENEMY_WALK_DOWN_FRAME_COUNT = 2;
constexpr uint ENEMY_WALK_DOWN_FRAMES[ENEMY_WALK_DOWN_FRAME_COUNT] = { 0,1 };

constexpr uint ENEMY_WALK_LEFT = 5;
constexpr uint ENEMY_WALK_LEFT_FRAME_COUNT = 2;
constexpr uint ENEMY_WALK_LEFT_FRAMES[ENEMY_WALK_LEFT_FRAME_COUNT] = { 4,5 };

constexpr uint ENEMY_WALK_UP = 6;
constexpr uint ENEMY_WALK_UP_FRAME_COUNT = 2;
constexpr uint ENEMY_WALK_UP_FRAMES[ENEMY_WALK_UP_FRAME_COUNT] = { 8,9 };

constexpr uint ENEMY_WALK_RIGHT = 7;
constexpr uint ENEMY_WALK_RIGHT_FRAME_COUNT = 2;
constexpr uint ENEMY_WALK_RIGHT_FRAMES[ENEMY_WALK_RIGHT_FRAME_COUNT] = { 12,13 };


constexpr uint ENEMY_LOOK_DOWN = 8;
constexpr uint ENEMY_LOOK_DOWN_FRAME_COUNT = 4;
constexpr uint ENEMY_LOOK_DOWN_FRAMES[ENEMY_LOOK_DOWN_FRAME_COUNT] = { 2,0,3,0 };

constexpr uint ENEMY_LOOK_LEFT = 9;
constexpr uint ENEMY_LOOK_LEFT_FRAME_COUNT = 4;
constexpr uint ENEMY_LOOK_LEFT_FRAMES[ENEMY_LOOK_LEFT_FRAME_COUNT] = { 6,4,7,4 };

constexpr uint ENEMY_LOOK_UP = 10;
constexpr uint ENEMY_LOOK_UP_FRAME_COUNT = 4;
constexpr uint ENEMY_LOOK_UP_FRAMES[ENEMY_LOOK_UP_FRAME_COUNT] = { 10,8,11,8 };

constexpr uint ENEMY_LOOK_RIGHT = 12;
constexpr uint ENEMY_LOOK_RIGHT_FRAME_COUNT = 4;
constexpr uint ENEMY_LOOK_RIGHT_FRAMES[ENEMY_LOOK_RIGHT_FRAME_COUNT] = { 14,12,15,12 };


constexpr uint ENEMY_HURT_DOWN = 13;
constexpr uint ENEMY_HURT_LEFT = 14;
constexpr uint ENEMY_HURT_UP = 15;
constexpr uint ENEMY_HURT_RIGHT = 16;



// ELEVATOR ANIMATIONS

constexpr uint ELEVATOR_CLOSED = 0;
constexpr uint ELEVATOR_CLOSED_FRAME_COUNT = 1;
constexpr uint ELEVATOR_CLOSED_FRAMES[ELEVATOR_CLOSED_FRAME_COUNT] = { 0 };

constexpr uint ELEVATOR_OPENING = 1;
constexpr uint ELEVATOR_OPENING_FRAME_COUNT = 4;
constexpr uint ELEVATOR_OPENING_FRAMES[ELEVATOR_OPENING_FRAME_COUNT] = { 0,1,2,3 };

constexpr uint ELEVATOR_OPEN = 2;
constexpr uint ELEVATOR_OPEN_FRAME_COUNT = 1;
constexpr uint ELEVATOR_OPEN_FRAMES[ELEVATOR_OPEN_FRAME_COUNT] = { 3 };