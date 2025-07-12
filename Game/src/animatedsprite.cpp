#include "precomp.h"

#include "tilesheet.h"

#include "animatedsprite.h"

void AnimatedSprite::Init(TileSheet* _tileSheet)
{
	tileSheet = _tileSheet;
}

void AnimatedSprite::Update(float deltaTime)
{
	timeSinceLastFrame += deltaTime;

	if (timeSinceLastFrame >= animations[currentAnimation].frameTime)
	{
		currentFrame = (currentFrame + 1) % animations[currentAnimation].frameCount;
		timeSinceLastFrame = 0.f;
	}
}

void AnimatedSprite::Draw(Surface& target, int2 position) const
{
	tileSheet->DrawTile(target, position, animations[currentAnimation].frames[currentFrame]);
}

void AnimatedSprite::AddAnimation(uint index, AnimationData animation)
{
	animations[index] = animation;
}

void AnimatedSprite::SetAnimation(uint animation, bool reset)
{
	if (animation != currentAnimation || reset)
	{
		currentAnimation = animation; 
		currentFrame = 0; 
		timeSinceLastFrame = 0.f;
	}
}

void AnimatedSprite::SetFrame(uint frame)
{
	currentFrame = frame;
}

uint AnimatedSprite::GetCurrentTileIndex() const
{
	return animations[currentAnimation].frames[currentFrame];
}

uint2 AnimatedSprite::GetSpriteSize() const
{
	return tileSheet->GetTileSize();
}

