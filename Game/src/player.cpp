#include "precomp.h"

#include "inventory.h"
#include "tilesheet.h"

#include "resourceManager.h"

#include "animatedsprite.h"
#include "aabb.h"
#include "gameobject.h" 

#include "textBox.h"
#include "entity.h"
#include "pickupable.h"
#include "enemySystem.h"

#include "tilemap.h"
#include "arrayList.h"
#include "level.h"
#include "objectPool.h"
#include "pathfinder.h"
#include "enemy.h"

#include "player.h"

#include "Core/Engine.h"


Player::Player()
	: Entity(AABB({ 10,1 }, { 22,30 }), SNAKE_MAX_HEALTH)
{
	sprite->AddAnimation(SNAKE_IDLE_UP, AnimationData(SNAKE_IDLE_UP_FRAME_COUNT, SNAKE_IDLE_UP_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_IDLE_DOWN, AnimationData(SNAKE_IDLE_DOWN_FRAME_COUNT, SNAKE_IDLE_DOWN_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_IDLE_LEFT, AnimationData(SNAKE_IDLE_LEFT_FRAME_COUNT, SNAKE_IDLE_LEFT_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_IDLE_RIGHT, AnimationData(SNAKE_IDLE_RIGHT_FRAME_COUNT, SNAKE_IDLE_RIGHT_FRAMES, TIME_PER_FRAME_ANIMATION));

	sprite->AddAnimation(SNAKE_WALK_UP, AnimationData(SNAKE_WALK_UP_FRAME_COUNT, SNAKE_WALK_UP_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_WALK_DOWN, AnimationData(SNAKE_WALK_DOWN_FRAME_COUNT, SNAKE_WALK_DOWN_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_WALK_LEFT, AnimationData(SNAKE_WALK_LEFT_FRAME_COUNT, SNAKE_WALK_LEFT_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_WALK_RIGHT, AnimationData(SNAKE_WALK_RIGHT_FRAME_COUNT, SNAKE_WALK_RIGHT_FRAMES, TIME_PER_FRAME_ANIMATION));

	sprite->AddAnimation(SNAKE_PUNCH_UP, AnimationData(SNAKE_PUNCH_UP_FRAME_COUNT, SNAKE_PUNCH_UP_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_PUNCH_DOWN, AnimationData(SNAKE_PUNCH_DOWN_FRAME_COUNT, SNAKE_PUNCH_DOWN_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_PUNCH_LEFT, AnimationData(SNAKE_PUNCH_LEFT_FRAME_COUNT, SNAKE_PUNCH_LEFT_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_PUNCH_RIGHT, AnimationData(SNAKE_PUNCH_RIGHT_FRAME_COUNT, SNAKE_PUNCH_RIGHT_FRAMES, TIME_PER_FRAME_ANIMATION));

	sprite->AddAnimation(SNAKE_KNEEL_UP, AnimationData(SNAKE_KNEEL_UP_FRAME_COUNT, SNAKE_KNEEL_UP_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_KNEEL_DOWN, AnimationData(SNAKE_KNEEL_DOWN_FRAME_COUNT, SNAKE_KNEEL_DOWN_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_KNEEL_LEFT, AnimationData(SNAKE_KNEEL_LEFT_FRAME_COUNT, SNAKE_KNEEL_LEFT_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_KNEEL_RIGHT, AnimationData(SNAKE_KNEEL_RIGHT_FRAME_COUNT, SNAKE_KNEEL_RIGHT_FRAMES, TIME_PER_FRAME_ANIMATION));

	sprite->AddAnimation(SNAKE_KNEEL_WALK_UP, AnimationData(SNAKE_KNEEL_WALK_UP_FRAME_COUNT, SNAKE_KNEEL_WALK_UP_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_KNEEL_WALK_DOWN, AnimationData(SNAKE_KNEEL_WALK_DOWN_FRAME_COUNT, SNAKE_KNEEL_WALK_DOWN_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_KNEEL_WALK_LEFT, AnimationData(SNAKE_KNEEL_WALK_LEFT_FRAME_COUNT, SNAKE_KNEEL_WALK_LEFT_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_KNEEL_WALK_RIGHT, AnimationData(SNAKE_KNEEL_WALK_RIGHT_FRAME_COUNT, SNAKE_KNEEL_WALK_RIGHT_FRAMES, TIME_PER_FRAME_ANIMATION));

	sprite->AddAnimation(SNAKE_CRAWL_IDLE_UP, AnimationData(SNAKE_CRAWL_IDLE_UP_FRAME_COUNT, SNAKE_CRAWL_IDLE_UP_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_CRAWL_IDLE_DOWN, AnimationData(SNAKE_CRAWL_IDLE_DOWN_FRAME_COUNT, SNAKE_CRAWL_IDLE_DOWN_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_CRAWL_IDLE_LEFT, AnimationData(SNAKE_CRAWL_IDLE_LEFT_FRAME_COUNT, SNAKE_CRAWL_IDLE_LEFT_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_CRAWL_IDLE_RIGHT, AnimationData(SNAKE_CRAWL_IDLE_RIGHT_FRAME_COUNT, SNAKE_CRAWL_IDLE_RIGHT_FRAMES, TIME_PER_FRAME_ANIMATION));

	sprite->AddAnimation(SNAKE_CRAWL_UP, AnimationData(SNAKE_CRAWL_UP_FRAME_COUNT, SNAKE_CRAWL_UP_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_CRAWL_DOWN, AnimationData(SNAKE_CRAWL_DOWN_FRAME_COUNT, SNAKE_CRAWL_DOWN_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_CRAWL_LEFT, AnimationData(SNAKE_CRAWL_LEFT_FRAME_COUNT, SNAKE_CRAWL_LEFT_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_CRAWL_RIGHT, AnimationData(SNAKE_CRAWL_RIGHT_FRAME_COUNT, SNAKE_CRAWL_RIGHT_FRAMES, TIME_PER_FRAME_ANIMATION));



	sprite->AddAnimation(SNAKE_GUN_IDLE_UP, AnimationData(SNAKE_GUN_IDLE_UP_FRAME_COUNT, SNAKE_GUN_IDLE_UP_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_GUN_IDLE_DOWN, AnimationData(SNAKE_GUN_IDLE_DOWN_FRAME_COUNT, SNAKE_GUN_IDLE_DOWN_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_GUN_IDLE_LEFT, AnimationData(SNAKE_GUN_IDLE_LEFT_FRAME_COUNT, SNAKE_GUN_IDLE_LEFT_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_GUN_IDLE_RIGHT, AnimationData(SNAKE_GUN_IDLE_RIGHT_FRAME_COUNT, SNAKE_GUN_IDLE_RIGHT_FRAMES, TIME_PER_FRAME_ANIMATION));


	sprite->AddAnimation(SNAKE_GUN_WALK_UP, AnimationData(SNAKE_GUN_WALK_UP_FRAME_COUNT, SNAKE_GUN_WALK_UP_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_GUN_WALK_DOWN, AnimationData(SNAKE_GUN_WALK_DOWN_FRAME_COUNT, SNAKE_GUN_WALK_DOWN_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_GUN_WALK_LEFT, AnimationData(SNAKE_GUN_WALK_LEFT_FRAME_COUNT, SNAKE_GUN_WALK_LEFT_FRAMES, TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(SNAKE_GUN_WALK_RIGHT, AnimationData(SNAKE_GUN_WALK_RIGHT_FRAME_COUNT, SNAKE_GUN_WALK_RIGHT_FRAMES, TIME_PER_FRAME_ANIMATION));

	sprite->AddAnimation(SNAKE_DEATH, AnimationData(SNAKE_DEATH_FRAME_COUNT, SNAKE_DEATH_FRAMES, .3f));


	// Collisions that have not been set

	fullCollider.topLeft.x = std::min(walkCollider.topLeft.x, std::min(crawlHorizontalCollider.topLeft.x, crawlVerticalCollider.topLeft.x));
	fullCollider.topLeft.y = std::min(walkCollider.topLeft.y, std::min(crawlHorizontalCollider.topLeft.y, crawlVerticalCollider.topLeft.y));

	fullCollider.bottomRight.x = std::max(walkCollider.bottomRight.x, std::max(crawlHorizontalCollider.bottomRight.x, crawlVerticalCollider.bottomRight.x));
	fullCollider.bottomRight.y = std::max(walkCollider.bottomRight.y, std::max(crawlHorizontalCollider.bottomRight.y, crawlVerticalCollider.bottomRight.y));

	fullColliderOffset = fullCollider.topLeft;
}

void Player::Init(const ResourceManager& resourceManager, Level* _level, ObjectPool* _objectPool, Inventory* _inventory, EnemySystem* _enemySystem, irrklang::ISoundEngine* _soundEngine)
{
	Entity::Init(resourceManager.getTileSheet(TileSheets::Snake), _level, _objectPool, _soundEngine);
	inputManager = &Engine::GetInstance().GetInput();
	inventory = _inventory;
	enemySystem = _enemySystem;
}

void Player::Reset()
{
	isDead = false;
	finishedDeath = false;
}

void Player::HandleInput()
{
	float speed = isCrawling ? playerCrawlSpeed : playerSpeed;
	velocity = float2(0.f);


	if (inputManager->IsKeyJustDown(GLFW_KEY_W))
	{
		HandleKeyDown(GLFW_KEY_W);
	}

	if (inputManager->IsKeyJustDown(GLFW_KEY_S))
	{
		HandleKeyDown(GLFW_KEY_S);
	}

	if (inputManager->IsKeyJustDown(GLFW_KEY_A))
	{
		HandleKeyDown(GLFW_KEY_A);
	}

	if (inputManager->IsKeyJustDown(GLFW_KEY_D))
	{
		HandleKeyDown(GLFW_KEY_D);
	}


	if (inputManager->IsKeyJustUp(GLFW_KEY_W))
	{
		HandleKeyUp(GLFW_KEY_W);
	}

	if (inputManager->IsKeyJustUp(GLFW_KEY_S))
	{
		HandleKeyUp(GLFW_KEY_S);
	}

	if (inputManager->IsKeyJustUp(GLFW_KEY_A))
	{
		HandleKeyUp(GLFW_KEY_A);
	}

	if (inputManager->IsKeyJustUp(GLFW_KEY_D))
	{
		HandleKeyUp(GLFW_KEY_D);
	}




	if (currentTurnTime > 0.f)
		return;

	bool allowHorizontalMovement = !isCrawling || !forceCrawl || !disallowHorizontalCrawl;
	bool allowVerticalMovement = !isCrawling || !forceCrawl || !disallowVerticalCrawl;

	int keyDown = GetFirstKeyDown();

	switch (keyDown)
	{
	case GLFW_KEY_W:
		if (allowVerticalMovement)
			wishDirection = Direction::Up;
		break;
	case GLFW_KEY_A:
		if (allowHorizontalMovement)
			wishDirection = Direction::Left;
		break;
	case GLFW_KEY_S:
		if (allowVerticalMovement)
			wishDirection = Direction::Down;
		break;
	case GLFW_KEY_D:
		if (allowHorizontalMovement)
			wishDirection = Direction::Right;
		break;

	default:
		break;
	}


	if (isCrawling && lookDirection != wishDirection)
	{
		lookDirection = wishDirection;
		isKneeling = true;
		currentTurnTime = turnTime;
	}
	else if (lookDirection == Direction::Left && wishDirection == Direction::Right)
	{
		lookDirection = Direction::Up;
		currentTurnTime = turnTime;
	}
	else if (lookDirection == Direction::Right && wishDirection == Direction::Left)
	{
		lookDirection = Direction::Down;
		currentTurnTime = turnTime;
	}
	else if (lookDirection == Direction::Down && wishDirection == Direction::Up)
	{
		lookDirection = Direction::Left;
		currentTurnTime = turnTime;
	}
	else if (lookDirection == Direction::Up && wishDirection == Direction::Down)
	{
		lookDirection = Direction::Right;
		currentTurnTime = turnTime;
	}
	else
	{
		lookDirection = wishDirection;
	}

	if (currentTurnTime > 0.f)
	{
		return;
	}



	if (keyDown == GLFW_KEY_A)
	{
		velocity = float2(-speed, 0.f);
		//isKneeling = isCrawling && (!allowHCrawl || lookDirection != direction::LEFT);
		isKneeling = isCrawling && !allowHCrawl;
		lookDirection = Direction::Left;

	}
	else if (keyDown == GLFW_KEY_D)
	{
		velocity = float2(speed, 0.f);
		//isKneeling = isCrawling && (!allowHCrawl || lookDirection != direction::RIGHT);
		isKneeling = isCrawling && !allowHCrawl;
		lookDirection = Direction::Right;
	}
	else if (keyDown == GLFW_KEY_W)
	{
		velocity = float2(0.f, -speed);
		//isKneeling = isCrawling && (!allowVCrawl || lookDirection != direction::UP);
		isKneeling = isCrawling && !allowVCrawl;
		lookDirection = Direction::Up;
	}
	else if (keyDown == GLFW_KEY_S)
	{
		velocity = float2(0.f, speed);
		//isKneeling = isCrawling && (!allowVCrawl || lookDirection != direction::DOWN) ;
		isKneeling = isCrawling && !allowVCrawl;
		lookDirection = Direction::Down;

	}

	if (inputManager->IsKeyJustDown(GLFW_KEY_C))
	{

		if (isCrawling && !forceCrawl)
		{
			isCrawling = false;
			currentTurnTime = turnTime;
		}
		else if (!isCrawling)
		{
			isCrawling = true;
			isKneeling = true;
			currentTurnTime = turnTime;

			soundEngine->play2D("assets/Sounds/Crawl.ogg");
		}

	}

	if (inputManager->IsKeyJustDown(GLFW_KEY_N) && punchCooldown <= 0.f && !isCrawling)
	{
		punchCooldown = SNAKE_PUNCH_COOLDOWN;
		punchState = PunchState::FirstFrame;
		currentTurnTime = SNAKE_PUNCH_FRAME_TIME;
		isCrawling = false;
	}


	if (inputManager->IsKeyJustDown(GLFW_KEY_SPACE) && !isCrawling && 
		inventory->GetSelectedWeapon() == ItemType::Pistol &&
		inventory->GetAmmoCount() > 0)
	{
		inventory->UseSelectedWeapon();
		
		int2 dir = int2(0);
		int2 spawnoffset = int2(0);
		
		switch (lookDirection)
		{
		case Direction::Up:
			dir.y = -1;
			spawnoffset = int2(15, 10);
			break;
		case Direction::Down:
			dir.y = 1;
			spawnoffset = int2(15, 13);
			break;
		case Direction::Left:
			dir.x = -1;
			spawnoffset = int2(7, 9);
			break;
		case Direction::Right:
			dir.x = 1;
			spawnoffset = int2(24, 9);
			break;
		}

		objectPool->AddBullet(GetPosition() + float2{static_cast<float>(spawnoffset.x),static_cast<float>(spawnoffset.y) }, float2(static_cast<float>(dir.x), static_cast<float>(dir.y)), true);
		enemySystem->SoundResponse(GetTileFeetPosition());
		soundEngine->play2D("assets/Sounds/Shoot.ogg");
	}
	

#ifdef PLAYER_POSITION_DEBUG
	if (inputManager->IsKeyJustDown(GLFW_KEY_B))
	{
		printf("Player position: %f, %f\n", round(position.x), round(position.y));
	}
#endif // PLAYER_POSITION_DEBUG



}

void Player::Update(float deltaTime)
{
	if (isDead)
	{
		if (sprite->GetFrameIndex() == static_cast<int>(SNAKE_DEATH_FRAME_COUNT) - 1)
		{
			finishedDeath = true;
		}


	}
	else
	{

		level->ResolvePlayerCollision(*this);

		SetPlayerStates();

		if (currentTurnTime > 0.f)
			currentTurnTime -= deltaTime;

		if (punchCooldown > 0.f)
			punchCooldown -= deltaTime;

		if (punchState == PunchState::FirstFrame)
		{
			punchState = PunchState::Animation;

			Entity* entityHit = objectPool->GetFirstEntityColliderNonPlayer(GeneratePunchHitBox());
			if (entityHit != nullptr)
			{
				entityHit->TakeDamage(SNAKE_PUNCH_DAMAGE);
				soundEngine->play2D("assets/Sounds/punchEnemy.ogg");
			}
			else
			{
				float2 centerHitBox = GeneratePunchHitBox().topLeft + GeneratePunchHitBox().bottomRight;
				centerHitBox = centerHitBox / 2.f;


				float2 temp = centerHitBox / static_cast<float>(TERRAIN_TILE_SIZE);

				if (!level->IsWalkable(level->GetColliderTile(int2{ static_cast<int>(temp.x),static_cast<int>(temp.y)})))
				{
					enemySystem->SoundResponse(GetTileFeetPosition());

					soundEngine->play2D("assets/Sounds/punchWall.ogg");
				}
				else
				{
					soundEngine->play2D("assets/Sounds/punchAir.ogg");
				}
			}

		}
		else if (punchState == PunchState::Animation && currentTurnTime <= 0.f)
		{
			punchState = PunchState::Finished;
		}


		SetPosition(position + velocity * deltaTime);
	}

	// setting animation

	UpdateAnimations();



	sprite->Update(deltaTime);

}

void Player::Draw(Surface& target, int2 offset) const
{
	Entity::Draw(target, offset);

	if (isCrawling && visualCrawl)
	{
		int2 positionInt = int2(static_cast<int>(position.x), static_cast<int>(position.y));

		int2 topLeftTile = int2(positionInt.x / TERRAIN_TILE_SIZE, positionInt.y / TERRAIN_TILE_SIZE);
		int2 bottomRightTile = int2((positionInt.x + tileSheet->GetCellSize().x) / TERRAIN_TILE_SIZE, (positionInt.y + tileSheet->GetCellSize().y) / TERRAIN_TILE_SIZE);

		for (int y = topLeftTile.y; y <= bottomRightTile.y; y++)
		{
			for (int x = topLeftTile.x; x <= bottomRightTile.x; x++)
			{
				CollisionTileType t = level->GetColliderTile(int2(x, y));
				switch (t)
				{
				case CollisionTileType::WALK_HALF_OVERLAY:
					level->DrawTopHalfTile(target, int2(x, y), offset);
					break;
				//case CollisionTileType::FULL_COLLISION:		// because we are not grid aligned
				case CollisionTileType::CRAWL_COLLISION:
				case CollisionTileType::CRAWL_ONLY_OVERLAY:
				case CollisionTileType::CRAWL_OVERLAY_NO_FORCE:
					level->DrawTile(target, int2(x, y), offset);
					break;
				case CollisionTileType::FORCED_CRAWL_OVERLAY:
					if (forceCrawl && isCrawling)
						level->DrawTile(target, int2(x, y), offset);
					break;
				case CollisionTileType::CRAWL_ONLY:
				default:
					break;
				}

			}
		}
	}
}

void Player::SetPosition(float2 newPosition)
{
	float2 offset = newPosition - position;

	walkCollider.setPosition(walkCollider.topLeft + offset);
	crawlHorizontalCollider.setPosition(crawlHorizontalCollider.topLeft + offset);
	crawlVerticalCollider.setPosition(crawlVerticalCollider.topLeft + offset);

	fullCollider.setPosition(fullCollider.topLeft + offset);

	Entity::SetPosition(newPosition);
}

void Player::CheckCollision(const AABB& otherAABB, CollisionTileType colliderType)
{
	switch (colliderType)
	{
	case CollisionTileType::FULL_COLLISION:
	case CollisionTileType::CRAWL_OVERLAY_NO_FORCE:
	
		ResolveCollision(walkCollider, otherAABB);
		break;

	case CollisionTileType::CRAWL_COLLISION:
		if (forceCrawl && isCrawling && !IsKneeling())
		{
			if (lookDirection == Direction::Up || lookDirection == Direction::Down)
				ResolveCollision(crawlVerticalCollider, otherAABB);
			else
				ResolveCollision(crawlHorizontalCollider, otherAABB);
		}
		
		break;
	case CollisionTileType::CRAWL_ONLY:
	case CollisionTileType::WALK_HALF_OVERLAY:
	case CollisionTileType::CRAWL_ONLY_OVERLAY:
		if (!isCrawling)
		{
			ResolveCollision(GetWalkAABB(), otherAABB);
		}
		break;
	}
}

void Player::ResolveCollision(const AABB& own, const AABB& otherAABB)
{

#ifdef  NOCLIP
	return;
#endif //  NOCLIP


	AABB playerColliderCopy = own;
	float2 playerVelocityCopy = GetVelocity();

	if (fabs(velocity.x) > fabs(velocity.y))
	{
		playerColliderCopy.resolveHorizontalCollisions(otherAABB, playerVelocityCopy);
		playerColliderCopy.resolveVerticalCollisions(otherAABB, playerVelocityCopy);
	}
	else
	{

		playerColliderCopy.resolveVerticalCollisions(otherAABB, playerVelocityCopy);
		playerColliderCopy.resolveHorizontalCollisions(otherAABB, playerVelocityCopy);
	}


	float2 offset = playerColliderCopy.topLeft - own.topLeft;

	SetPosition(GetPosition() + offset);
	SetVelocity(playerVelocityCopy);

}

void Player::DrawDebug(Surface& target, int2 offset) const
{

	walkCollider.draw(target, level->GetWorldOffset() + offset, 0xff1432ff);			// red

	crawlHorizontalCollider.draw(target, level->GetWorldOffset() + offset, 0xff49aeed); // yellow
	crawlVerticalCollider.draw(target, level->GetWorldOffset() + offset, 0xff8c7900); // blue

	fullCollider.draw(target, level->GetWorldOffset() + offset, 0xfffff9c9); // light blue

	int2 positionInInt = int2(static_cast<int>(position.x), static_cast<int>(position.y));

	target.Box(positionInInt.x + level->GetWorldOffset().x + offset.x, positionInInt.y + level->GetWorldOffset().y + offset.y,
		positionInInt.x + tileSheet->GetCellSize().x + level->GetWorldOffset().x + offset.x, positionInInt.y + tileSheet->GetCellSize().y + level->GetWorldOffset().y + offset.y, 0xff20f0f0);


	if (punchState == PunchState::Animation)
	{

		GeneratePunchHitBox().draw(target, level->GetWorldOffset() + offset, 0xff020fff);
	}
}

bool Player::IsCrawling() const
{
	return isCrawling;
}

bool Player::IsKneeling() const
{
	return isKneeling;
}

const AABB& Player::GetFullCollider() const
{
	return fullCollider;
}

const AABB& Player::GetActiveCollider() const
{
	if (isCrawling)
	{

		if (isKneeling)
		{
			return walkCollider;
		}

		switch (lookDirection)
		{
		case Direction::Up:
		case Direction::Down:
			return crawlVerticalCollider;

		case Direction::Left:
		case Direction::Right:
		default:
			return crawlHorizontalCollider;
		}

	}
	else
	{
		return walkCollider;
	}
}

const AABB& Player::GetCrawlVCollider() const
{
	return crawlVerticalCollider;
}

const AABB& Player::GetCrawlHCollider() const
{
	return crawlHorizontalCollider;
}

void Player::HandleKeyDown(int key)
{

	if (!(key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D))
		return;

	if (!isKeyInStack(key) && stackTop < 3) {
		keyStack[++stackTop] = key;
	}
}

void Player::HandleKeyUp(int key)
{
	if (!(key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D))
		return;

	for (int i = 0; i <= stackTop; ++i) {
		if (keyStack[i] == key) {
			// we shift all things to the right of the key we remove to the left, and thus overwriting it too.

			for (int j = i; j < 3; ++j) {
				keyStack[j] = keyStack[j + 1];
			}
			stackTop--;
			break;
		}
	}

}



int Player::GetFirstKeyDown() const
{
	if (stackTop != -1)
		return keyStack[stackTop];

	return 0;
}

void Player::SetFeetPosition(float2 _position)
{
	SetPosition(_position - (GetFeetPosition() - GetPosition()));
}

float2 Player::GetFeetPosition() const
{
	return GetPosition() + float2(static_cast<float>(tileSheet->GetCellSize().x / 2u), static_cast<float>(tileSheet->GetCellSize().y - 2u));
}

int2 Player::GetTileFeetPosition() const
{
	return int2(int(floor((GetFeetPosition().x) / static_cast<float>(TERRAIN_TILE_SIZE))), int(floor((GetFeetPosition().y) / static_cast<float>(TERRAIN_TILE_SIZE))));
}

bool Player::IsForcedToCrawl() const
{
	return forceCrawl;
}

Inventory* Player::GetInventory() const
{
	return inventory;
}

void Player::ResetCrawlDisallow()
{

	disallowVerticalCrawl = false;
	disallowHorizontalCrawl = false;
}

void Player::SetCrawlDisallowVertical()
{
	disallowVerticalCrawl = true;
}

void Player::SetCrawlDisallowHorizontal()
{
	disallowHorizontalCrawl = true;
}

void Player::SetLookDirection(Direction dir)
{
	lookDirection = dir;
}

Direction Player::GetLookDirection() const
{
	return lookDirection;
}

void Player::TakeDamage(int damage)
{
	Entity::TakeDamage(damage);
	soundEngine->play2D("assets/Sounds/PlayerDamage.ogg");
}

bool Player::IsDead() const
{
	return isDead;
}

bool Player::DeathCompleted() const
{
	return finishedDeath;
}

void Player::OnDeath()
{

	if (inventory->GetSelectedEquipment() == ItemType::Ration)
	{
		SetCurrentHealth(GetMaxHealth());
		inventory->UseSelectedEquipment();
		return;
	}

	isDead = true;

	soundEngine->play2D("assets/Sounds/PlayerDeath.ogg");
	printf("Player DIED!\n");
}

void Player::SetPlayerStates()
{
	AABB playerAABB = GetFullCollider();

	forceCrawl = false;
	visualCrawl = false;
	allowHCrawl = true;
	allowVCrawl = true;


	int2 topLeftTile = int2(static_cast<int>(playerAABB.topLeft.x) / static_cast<int>(TERRAIN_TILE_SIZE),
		static_cast<int>(playerAABB.topLeft.y) / static_cast<int>(TERRAIN_TILE_SIZE));

	topLeftTile.x = std::min(std::max(topLeftTile.x, 0), static_cast<int>(level->GetLevelSize().x - 1));
	topLeftTile.y = std::min(std::max(topLeftTile.y, 0), static_cast<int>(level->GetLevelSize().y - 1));

	int2 bottomRightTile = int2(static_cast<int>(playerAABB.bottomRight.x) / static_cast<int>(TERRAIN_TILE_SIZE),
		static_cast<int>(playerAABB.bottomRight.y) / static_cast<int>(TERRAIN_TILE_SIZE));

	bottomRightTile.x = std::min(std::max(bottomRightTile.x, 0), static_cast<int>(level->GetLevelSize().x - 1));
	bottomRightTile.y = std::min(std::max(bottomRightTile.y, 0), static_cast<int>(level->GetLevelSize().y - 1));


	for (int y = topLeftTile.y; y <= bottomRightTile.y; y++)
	{
		for (int x = topLeftTile.x; x <= bottomRightTile.x; x++)
		{
			CollisionTileType tileType = static_cast<CollisionTileType>(level->GetColliderTile(int2(x, y)));
			if (tileType == CollisionTileType::WALKABLE_OPEN)
				continue;

			float2 tilePosition = float2(static_cast<float>(x * TERRAIN_TILE_SIZE), static_cast<float>(y * TERRAIN_TILE_SIZE));
			AABB tileAABB = AABB(tilePosition, tilePosition + float2(TERRAIN_TILE_SIZE - 1));



			if ((tileType == CollisionTileType::CRAWL_ONLY ||
				tileType == CollisionTileType::CRAWL_ONLY_OVERLAY)
				&& (GetCrawlHCollider().intersect(tileAABB) || GetCrawlVCollider().intersect(tileAABB)))
			{
				visualCrawl = true;
			}


			if (tileType == CollisionTileType::CRAWL_ONLY || tileType == CollisionTileType::CRAWL_ONLY_OVERLAY)
				forceCrawl = true;
		

			if (tileType == CollisionTileType::FULL_COLLISION && GetCrawlHCollider().intersect(tileAABB))
			{
				allowHCrawl = false;
			}

			if (tileType == CollisionTileType::FULL_COLLISION && GetCrawlVCollider().intersect(tileAABB))
			{
				allowVCrawl = false;
			}
		
		}
	}
}



bool Player::isKeyInStack(int key) const
{
	for (int i = 0; i < 4; i++)
	{
		if (keyStack[i] == key)
			return true;
	}
	return false;
}

void Player::SetAnimationBasedDirection(uint UP, uint DOWN, uint LEFT, uint RIGHT)
{
	switch (lookDirection)
	{
	case Direction::Up:
		sprite->SetAnimation(UP);
		break;
	case Direction::Down:
		sprite->SetAnimation(DOWN);
		break;
	case Direction::Left:
		sprite->SetAnimation(LEFT);
		break;
	case Direction::Right:
		sprite->SetAnimation(RIGHT);
		break;
	}
}

void Player::UpdateAnimations()
{
	if (isDead)
	{
		sprite->SetAnimation(SNAKE_DEATH);
		return;
	}


	if (punchState == PunchState::Animation)
	{
		SetAnimationBasedDirection(SNAKE_PUNCH_UP, SNAKE_PUNCH_DOWN, SNAKE_PUNCH_LEFT, SNAKE_PUNCH_RIGHT);
	}
	else if (isCrawling && isKneeling)
	{

		if (lengthSq(velocity) == 0.f)
		{
			SetAnimationBasedDirection(SNAKE_KNEEL_UP, SNAKE_KNEEL_DOWN, SNAKE_KNEEL_LEFT, SNAKE_KNEEL_RIGHT);
		}
		else
		{
			SetAnimationBasedDirection(SNAKE_KNEEL_WALK_UP, SNAKE_KNEEL_WALK_DOWN, SNAKE_KNEEL_WALK_LEFT, SNAKE_KNEEL_WALK_RIGHT);
		}


	}
	else if (isCrawling && !isKneeling)
	{

		if (lengthSq(velocity) == 0.f)
		{
			SetAnimationBasedDirection(SNAKE_CRAWL_IDLE_UP, SNAKE_CRAWL_IDLE_DOWN, SNAKE_CRAWL_IDLE_LEFT, SNAKE_CRAWL_IDLE_RIGHT);
		}
		else
		{
			SetAnimationBasedDirection(SNAKE_CRAWL_UP, SNAKE_CRAWL_DOWN, SNAKE_CRAWL_LEFT, SNAKE_CRAWL_RIGHT);
		}


	}
	else if (!isCrawling)
	{

		if (lengthSq(velocity) == 0.f)
		{
			if (inventory->GetSelectedWeapon() == ItemType::Pistol)
				SetAnimationBasedDirection(SNAKE_GUN_IDLE_UP, SNAKE_GUN_IDLE_DOWN, SNAKE_GUN_IDLE_LEFT, SNAKE_GUN_IDLE_RIGHT);
			else
				SetAnimationBasedDirection(SNAKE_IDLE_UP, SNAKE_IDLE_DOWN, SNAKE_IDLE_LEFT, SNAKE_IDLE_RIGHT);
		}
		else
		{
			if (inventory->GetSelectedWeapon() == ItemType::Pistol)
				SetAnimationBasedDirection(SNAKE_GUN_WALK_UP, SNAKE_GUN_WALK_DOWN, SNAKE_GUN_WALK_LEFT, SNAKE_GUN_WALK_RIGHT);
			else
				SetAnimationBasedDirection(SNAKE_WALK_UP, SNAKE_WALK_DOWN, SNAKE_WALK_LEFT, SNAKE_WALK_RIGHT);
		}
	}
}

AABB Player::GeneratePunchHitBox() const
{
	// THIS IS UP
	float2 positionLeftTop = float2(10, -5);
	float2 positionBottomRight = float2(20, 5);
	if (lookDirection == Direction::Left)
	{
		positionLeftTop = float2(0, 5);
		positionBottomRight = float2(10, 15);
	}
	else if (lookDirection == Direction::Right)
	{
		positionLeftTop = float2(20, 5);
		positionBottomRight = float2(30, 15);
	}
	else if (lookDirection == Direction::Down)
	{
		positionLeftTop = float2(10, 20);
		positionBottomRight = float2(20, 30);
	}



	return AABB(positionLeftTop + GetPosition(), positionBottomRight + GetPosition());
}
