#include "precomp.h"

#include "resourceManager.h"


#include "textBox.h"
#include "tilesheet.h"
#include "font.h"

#include "aabb.h"
#include "animatedsprite.h"
#include "gameobject.h"
#include "entity.h"
#include "player.h"

#include "tilemap.h"
#include "arrayList.h"
#include "level.h"

#include "arrayList.h"
#include "pathfinder.h"
#include "enemySystem.h"

#include "objectPool.h"

#include "enemy.h"

Enemy::Enemy()
	: Entity(AABB({1.f,0.f}, {13.f,31.f}), ENEMY_MAX_HEALTH)
{
	sprite->AddAnimation(ENEMY_IDLE_DOWN,	AnimationData(ENEMY_IDLE_DOWN_FRAME_COUNT,	ENEMY_IDLE_DOWN_FRAMES,		TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(ENEMY_IDLE_LEFT,	AnimationData(ENEMY_IDLE_LEFT_FRAME_COUNT,	ENEMY_IDLE_LEFT_FRAMES,		TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(ENEMY_IDLE_UP,		AnimationData(ENEMY_IDLE_UP_FRAME_COUNT,	ENEMY_IDLE_UP_FRAMES,		TIME_PER_FRAME_ANIMATION));
	sprite->AddAnimation(ENEMY_IDLE_RIGHT,	AnimationData(ENEMY_IDLE_RIGHT_FRAME_COUNT, ENEMY_IDLE_RIGHT_FRAMES,	TIME_PER_FRAME_ANIMATION));

	sprite->AddAnimation(ENEMY_WALK_DOWN,	AnimationData(ENEMY_WALK_DOWN_FRAME_COUNT,	ENEMY_WALK_DOWN_FRAMES,		.3f));
	sprite->AddAnimation(ENEMY_WALK_LEFT,	AnimationData(ENEMY_WALK_LEFT_FRAME_COUNT,	ENEMY_WALK_LEFT_FRAMES,		.3f));
	sprite->AddAnimation(ENEMY_WALK_UP,		AnimationData(ENEMY_WALK_UP_FRAME_COUNT,ENEMY_WALK_UP_FRAMES,			.3f));
	sprite->AddAnimation(ENEMY_WALK_RIGHT,	AnimationData(ENEMY_WALK_RIGHT_FRAME_COUNT, ENEMY_WALK_RIGHT_FRAMES,	.3f));

	sprite->AddAnimation(ENEMY_LOOK_DOWN,	AnimationData(ENEMY_LOOK_DOWN_FRAME_COUNT,	ENEMY_LOOK_DOWN_FRAMES,		.3f));
	sprite->AddAnimation(ENEMY_LOOK_LEFT,	AnimationData(ENEMY_LOOK_LEFT_FRAME_COUNT,	ENEMY_LOOK_LEFT_FRAMES,		.3f));
	sprite->AddAnimation(ENEMY_LOOK_UP,		AnimationData(ENEMY_LOOK_UP_FRAME_COUNT,	ENEMY_LOOK_UP_FRAMES,		.3f));
	sprite->AddAnimation(ENEMY_LOOK_RIGHT,	AnimationData(ENEMY_LOOK_RIGHT_FRAME_COUNT, ENEMY_LOOK_RIGHT_FRAMES,	.3f));

	sprite->AddAnimation(ENEMY_HURT_DOWN, AnimationData(ENEMY_LOOK_DOWN_FRAME_COUNT, ENEMY_LOOK_DOWN_FRAMES, .1f));
	sprite->AddAnimation(ENEMY_HURT_LEFT, AnimationData(ENEMY_LOOK_LEFT_FRAME_COUNT, ENEMY_LOOK_LEFT_FRAMES, .1f));
	sprite->AddAnimation(ENEMY_HURT_UP, AnimationData(ENEMY_LOOK_UP_FRAME_COUNT, ENEMY_LOOK_UP_FRAMES, .1f));
	sprite->AddAnimation(ENEMY_HURT_RIGHT, AnimationData(ENEMY_LOOK_RIGHT_FRAME_COUNT, ENEMY_LOOK_RIGHT_FRAMES, .1f));

}

Enemy::~Enemy()
{
	enemySystem->RemoveEnemy(this);

	delete[] patrolList;
}

void Enemy::Init(const ResourceManager& resourceManager, Level* _level, ObjectPool* _objectPool, EnemySystem* _enemySystem, Player* _player, irrklang::ISoundEngine* _soundEngine)
{
	Entity::Init(resourceManager.getTileSheet(TileSheets::Enemy), _level, _objectPool, _soundEngine);
	enemySystem = _enemySystem;
	player = _player;
	enemyIcon = resourceManager.getTileSheet(TileSheets::EnemyIcon);

	textBox.Init(resourceManager);

	enemySystem->AddEnemyPointer(this);
}

void Enemy::CreatePatrolList(int length, PatrolAction* list)
{
	enemyType = EnemyType::Patrol;

	delete[] patrolList;
	patrolList = new PatrolAction[length];
	patrolListLength = length;
	for (int i = 0; i < length; i++)
	{
		patrolList[i].type = list[i].type;
		patrolList[i].value = list[i].value;
		patrolList[i].tilePosition = list[i].tilePosition;
	}
	
}

// overrides the Entity take damage function
void Enemy::TakeDamage(int damage) 
{
	// stun
	stunnedDuration = ENEMY_DAMAGE_STUN_DURATION;

	Entity::TakeDamage(damage);
}





void Enemy::Update(float deltaTime)
{
	if (IsDisabled())
		return;


	if (enemyIconShowTime > 0.f)
		enemyIconShowTime -= deltaTime;

	if (textBoxShowTime > 0.f)
		textBoxShowTime -= deltaTime;


	if (stunnedDuration > 0.f)
	{
		stunnedDuration -= deltaTime;
		if (stunnedDuration <= 0.f)
		{
			enemySystem->ActivateAlarm();
		}
	}


	sprite->Update(deltaTime);
	velocity = float2(0.f);


	if (stunnedDuration <= 0.f)
	{

		if (confusedTime > 0.f)
		{
			confusedTime -= deltaTime;

			if (confusedTime <= (ENEMY_CONFUSE_TIME - 1.f) && confusedTime > 1.f)
			{
				if (FollowPath(deltaTime))
				{
					confusedLookTime -= deltaTime;
					if (confusedLookTime <= 0.f)
					{
						confusedLookTime = .5f;
						switch (lookDirection)
						{
						case Direction::Up:
							lookDirection = Direction::Right;
							viewDirection = float2(1.f, 0.f);
							break;
						case Direction::Down:
							lookDirection = Direction::Left;
							viewDirection = float2(-1.f, 0.f);
							break;
						case Direction::Left:
							lookDirection = Direction::Up;
							viewDirection = float2(0.f, -1.f);
							break;
						case Direction::Right:
							lookDirection = Direction::Down;
							viewDirection = float2(0.f, 1.f);
							break;
						}
					}
				}
			}

			else if (confusedTime <= (ENEMY_CONFUSE_TIME - 1.f) && confusedTime > .1f)
			{
				if (textBoxShowTime <= 0.f)
				{
					ShowTextBox(1.f, "NOT HERE");
				}
			}

			if (confusedTime <= 0.f)
			{
				SetPath(pathfinder.CalculatePath(*level,GetTileFeetPosition(), oldPosition));
			}
		}
		else if (!confusedFinished)
		{
			confusedFinished = FollowPath(deltaTime);
		}
		else
		{

			switch (enemyType)
			{
			case EnemyType::Patrol:
				UpdatePatrol(deltaTime);
				break;
			case EnemyType::Search:
				UpdateSearch(deltaTime);
				break;
			default:
				break;
			}
		}


		if (velocity.x > 0.f && abs(velocity.x) > abs(velocity.y))
		{
			lookDirection = Direction::Right;

		}
		else if (velocity.x < 0.f && abs(velocity.x) > abs(velocity.y))
		{
			lookDirection = Direction::Left;
		
		}
		else if (velocity.y > 0.f && abs(velocity.x) < abs(velocity.y))
		{
			lookDirection = Direction::Down;
		}
		else if (velocity.y < 0.f && abs(velocity.x) < abs(velocity.y))
		{
			lookDirection = Direction::Up;
		}
	}

	UpdateAnimationState();

	SetPosition(position + velocity * deltaTime);
	CheckPlayerVisibility(*player);


}

void Enemy::Draw(Surface& target, int2 offset) const
{
	Entity::Draw(target, offset);

	if (enemyIconShowTime > 0.f)
	{
		
		enemyIcon->DrawTile(target, level->worldToScreenSpace(GetPosition()) + offset + int2(8, -12), enemyIconFrame);
	}

	if (textBoxShowTime > 0.f)
	{
		textBox.Draw(target, level->worldToScreenSpace(GetPosition()) + offset + int2(4, -14));
	}
}

void Enemy::DrawDebug(Surface& target, int2 offset) const
{
	if (IsDisabled())
		return;

	GetCollider().draw(target, level->GetWorldOffset() + offset, 0xffffffff);
}

void Enemy::OnDeath()
{
	Entity::OnDeath();
	soundEngine->play2D("assets/Sounds/EnemyDeath.ogg");
}

void Enemy::SetPosition(float2 newPosition)
{
	Entity::SetPosition(newPosition);
}



bool Enemy::FollowPath(float deltaTime)
{

	if (currentPathIndex >= pathNodes.size())
		return true;

	int2 currentNode = pathNodes.at(currentPathIndex);

	float2 direction = float2(float(currentNode.x*TERRAIN_TILE_SIZE + TERRAIN_TILE_SIZE/2u), float(currentNode.y * TERRAIN_TILE_SIZE + TERRAIN_TILE_SIZE / 2u)) - GetFeetPosition();
	float distance = length(direction);

	// small error offset for floating point precision error, 
	// since if we would move over the point velocity will be smaller. (CalculateVelocityMagnitude)
	if (distance <= 0.005f) 
	{
		currentPathIndex++;
		if (currentPathIndex >= pathNodes.size())
			return true;

		currentNode = pathNodes.at(currentPathIndex);

		const int2 temp = currentNode - GetTileFeetPosition();
		direction = float2(static_cast<float>(temp.x), static_cast<float>(temp.y));
		distance = length(direction);

	}
	
	velocity = float2(direction.x/distance, direction.y / distance) * CalculateVelocityMagnitude(deltaTime, distance);
	

	return false;
}

float2 Enemy::GetFeetPosition() const
{
	return GetPosition() + float2(static_cast<float>(tileSheet->GetCellSize().x / 2u), static_cast<float>(tileSheet->GetCellSize().y - 2u));
}

int2 Enemy::GetTileFeetPosition() const
{
	return int2(int(floor((GetFeetPosition().x) / static_cast<float>(TERRAIN_TILE_SIZE))), int(floor((GetFeetPosition().y) / static_cast<float>(TERRAIN_TILE_SIZE))));
}

void Enemy::SetPath(const ArrayList<int2>& path)
{
	pathNodes = path;
	currentPathIndex = 0;
}

void Enemy::SetConfused(int2 soundPositionTile)
{
	confusedTime = ENEMY_CONFUSE_TIME;
	confusedFinished = false;

	ShowEnemyIcon(1.f, 0);
	SetPath(pathfinder.CalculatePath(*level, GetTileFeetPosition(), soundPositionTile));
}

void Enemy::ShowEnemyIcon(float duration, int frame)
{
	enemyIconShowTime = duration;
	enemyIconFrame = frame;
}

void Enemy::ShowTextBox(float duration, const char* text)
{
	textBoxShowTime = duration;
	textBox.SetText(text);
	textBox.SetSize(int2(static_cast<int>(strlen(text)) * textBox.GetFont()->GetCharacterSize().x + 7, 18));
}

float Enemy::CalculateVelocityMagnitude(float deltaTime, float distance) const
{
	float calculatedSpeed = distance / deltaTime;


	return std::min(moveSpeed, calculatedSpeed);
}

void Enemy::UpdatePatrol(float deltaTime)
{

	PatrolActionType patrolActionType = patrolList[currentAction].type;

	switch (patrolActionType)
	{
	case PatrolActionType::Nothing:
		break;
	case PatrolActionType::MoveTo:
		if (pathNodes.size() == 0)
		{			
			pathNodes = pathfinder.CalculatePath(*level, GetTileFeetPosition(), patrolList[currentAction].tilePosition);
			currentPathIndex = 0;
		}

		if (!FollowPath(deltaTime))
		{
			if (abs(velocity.x) > abs(velocity.y))
			{
				viewDirection = float2(velocity.x > 0.f ? 1.f : -1.f, 0.f);
			}
			else if (abs(velocity.x) < abs(velocity.y))
			{
				viewDirection = float2(0.f, velocity.y > 0.f ? 1.f : -1.f);
			}
			return;
		}
		break;
	case PatrolActionType::IdleDown:
		viewDirection = float2(0.f, 1.f);
		break;
	case PatrolActionType::IdleLeft:
		viewDirection = float2(-1.f, 0.f);
		break;
	case PatrolActionType::IdleUp:
		viewDirection = float2(0.f, -1.f);
		break;
	case PatrolActionType::IdleRight:
		viewDirection = float2(1.f, 0.f);
		break;
	case PatrolActionType::LookDown:

		if (sprite->GetFrameIndex() == 0)
		{
			viewDirection = float2(1.f, 1.f);
		}
		else if (sprite->GetFrameIndex() == 2)
		{
			viewDirection = float2(-1.f, 1.f);
		}
		else
		{
			viewDirection = float2(0.f, 1.f);
		}
		break;
	case PatrolActionType::LookLeft:
		if (sprite->GetFrameIndex() == 0)
		{
			viewDirection = float2(-1.f, 1.f);
		}
		else if (sprite->GetFrameIndex() == 2)
		{
			viewDirection = float2(-1.f, -1.f);
		}
		else
		{
			viewDirection = float2(-1.f, 0.f);
		}
		break;
	case PatrolActionType::LookUp:
		

		if (sprite->GetFrameIndex() == 0)
		{
			viewDirection = float2(-1.f, -1.f);
		}
		else if (sprite->GetFrameIndex() == 2)
		{
			viewDirection = float2(1.f, -1.f);
		}
		else
		{
			viewDirection = float2(0.f, -1.f);
		}
		break;
	case PatrolActionType::LookRight:
		if (sprite->GetFrameIndex() == 0)
		{
			viewDirection = float2(1.f, -1.f);
		}
		else if (sprite->GetFrameIndex() == 2)
		{
			viewDirection = float2(1.f, 1.f);
		}
		else
		{
			viewDirection = float2(1.f, 0.f);
		}
		break;
	default:
		break;
	}


	currentValue += deltaTime;
	

	if (patrolList[currentAction].type == PatrolActionType::Nothing || currentValue >= patrolList[currentAction].value)
	{
		pathNodes.clear();
		currentPathIndex = 0;

		currentAction= (currentAction+1)%patrolListLength;
		currentValue = 0.f;
	}


}

void Enemy::UpdateSearch(float deltaTime)
{

	if (shootRange)
	{
		// enemy can shoot we reset their path as well so after shooting it does not walk to a old position
		pathNodes.clear();		
		if (shootCountdown <= 0.f)
		{
			ShootToPlayer();
			shootCountdown = ENEMY_SHOOT_DELAY;
		}
		else
		{
			shootCountdown -= deltaTime;
		}
	}
	else
	{
		bool finishedPath = false;
		if (pathNodes.size() != 0)
		{
			finishedPath = FollowPath(deltaTime);
		}
		
		if (pathNodes.size() == 0 || finishedPath)
		{
			enemySystem->SetEnemyGoal(this);
		}
	}

	if (abs(velocity.x) > abs(velocity.y))
	{
		viewDirection = float2(velocity.x > 0.f ? 1.f : -1.f, 0.f);
	}
	else if (abs(velocity.x) < abs(velocity.y))
	{
		viewDirection = float2(0.f, velocity.y > 0.f ? 1.f : -1.f);
	}
}

void Enemy::UpdateAnimationState()

{
	if (stunnedDuration > 0.f)
	{
		switch (lookDirection)
		{
		case Direction::Up:
			sprite->SetAnimation(ENEMY_HURT_UP);
			break;
		case Direction::Down:
			sprite->SetAnimation(ENEMY_HURT_DOWN);
			break;
		case Direction::Left:
			sprite->SetAnimation(ENEMY_HURT_LEFT);
			break;
		case Direction::Right:
			sprite->SetAnimation(ENEMY_HURT_RIGHT);
			break;
		default:
			break;
		}
	}

	else if (lengthSq(velocity) == 0.f)
	{

		if (enemyType == EnemyType::Patrol && patrolList[currentAction].type != PatrolActionType::MoveTo && confusedTime <= 0.f)
		{
			switch (patrolList[currentAction].type)
			{
			case PatrolActionType::IdleDown:
				sprite->SetAnimation(ENEMY_IDLE_DOWN);
				break;
			case PatrolActionType::IdleLeft:
				sprite->SetAnimation(ENEMY_IDLE_LEFT);
				break;
			case PatrolActionType::IdleUp:
				sprite->SetAnimation(ENEMY_IDLE_UP);
				break;
			case PatrolActionType::IdleRight:
				sprite->SetAnimation(ENEMY_IDLE_RIGHT);
				break;
			case PatrolActionType::LookDown:
				sprite->SetAnimation(ENEMY_LOOK_DOWN);
				break;
			case PatrolActionType::LookLeft:
				sprite->SetAnimation(ENEMY_LOOK_LEFT);
				break;
			case PatrolActionType::LookUp:
				sprite->SetAnimation(ENEMY_LOOK_UP);
				break;
			case PatrolActionType::LookRight:
				sprite->SetAnimation(ENEMY_LOOK_RIGHT);
				break;
			default:
				break;
			}
		}
		else
		{

			switch (lookDirection)
			{
			case Direction::Up:
				sprite->SetAnimation(ENEMY_IDLE_UP);
				break;
			case Direction::Down:
				sprite->SetAnimation(ENEMY_IDLE_DOWN);
				break;
			case Direction::Left:
				sprite->SetAnimation(ENEMY_IDLE_LEFT);
				break;
			case Direction::Right:
				sprite->SetAnimation(ENEMY_IDLE_RIGHT);
				break;
			default:
				break;
			}

		}




	}
	else
	{
		switch (lookDirection)
		{
		case Direction::Up:
			sprite->SetAnimation(ENEMY_WALK_UP);
			break;
		case Direction::Down:
			sprite->SetAnimation(ENEMY_WALK_DOWN);
			break;
		case Direction::Left:
			sprite->SetAnimation(ENEMY_WALK_LEFT);
			break;
		case Direction::Right:
			sprite->SetAnimation(ENEMY_WALK_RIGHT);
			break;
		default:
			break;
		}
	}


}

void Enemy::CheckPlayerVisibility(Player& _player)
{
	if (!IsVisible())
	{
		playerVisible = false;
		return;
	}

#ifdef UNSEEABLE
	playerVisible = false;
	return;
#endif // UNSEEABLE


	// changed the get position so this may be broken

	/*float2 centerEnemy = GetPosition() + float2(static_cast<float>(tileSheet->GetCellSize().x / 2u), static_cast<float>(ENEMY_CELL_SIZE.y-4u));
	float2 centerPlayer = _player.GetPosition() + float2(_player.get / 2u, SNAKE_CELL_SIZE-2u);*/
	float2 centerEnemy = GetFeetPosition();
	float2 centerPlayer = _player.GetFeetPosition();





	float dotProduct = dot(normalize(viewDirection), normalize(centerPlayer - centerEnemy));

	bool inVisionCone = dotProduct > 1.f/sqrt(2.f);

	float2 hitpoint;
	bool visable = inVisionCone && level->RaycastCollision(centerEnemy, centerPlayer, hitpoint);
	
	if (visable)
	{
		enemySystem->ActivateAlarm();
		playerVisible = true;
		shootRange = lengthSq(centerEnemy - centerPlayer) < ENEMY_SHOOT_DISTANCE * ENEMY_SHOOT_DISTANCE;
	}
	else
	{
		playerVisible = false;
		shootRange = false;
	}

}

void Enemy::ShootToPlayer()
{
	float2 spawnPosition = GetBulletSpawnPosition();

	float2 shootTarget = player->GetFeetPosition() + float2(0, -BULLET_TERRAIN_HITBOX_OFFSET);
	if (player->IsCrawling())
	{
		shootTarget.y += 4.f;
		if (!player->IsKneeling())
			shootTarget.y += 4.f;
	}
	soundEngine->play2D("assets/Sounds/Shoot.ogg");
	objectPool->AddBullet(spawnPosition, shootTarget - spawnPosition, false);
}

float2 Enemy::GetBulletSpawnPosition()
{
	float2 offset = float2(9.f, 15.f);

	switch (lookDirection)
	{
	case Direction::Up:
		offset = float2(6.f, 8.f);
		break;
	case Direction::Left:
		offset = float2(0.f, 7.f);
		break;
	case Direction::Right:
		offset = float2(14.f, 10.f);
		break;
	}

	return GetPosition() + offset;
}
