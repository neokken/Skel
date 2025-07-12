#pragma once

class TileSheet;


struct AnimationData
{
	AnimationData() {}
	AnimationData(uint _frameCount,const uint _frames[],  float _frameTime)
	{
		for (uint i = 0; i < _frameCount; i++)
		{
			frames[i] = _frames[i];
		}
		frameCount = _frameCount;
		frameTime = _frameTime;
	}
	uint frames[MAX_ANIMATION_FRAMES]{0};
	uint frameCount{ 0 };
	float frameTime{ .1f };
};



class AnimatedSprite
{
public:
	AnimatedSprite() {};

	void Init(TileSheet* tileSheet);


	void Update(float deltaTime);

	void Draw(Surface& target, int2 position) const;

	void AddAnimation(uint index, AnimationData animation);

	int GetAnimation() const { return currentAnimation; }

	void SetAnimation(uint animation, bool reset = false);

	void SetFrame(uint frame);

	int GetFrameIndex() const { return currentFrame; }

	uint GetCurrentTileIndex() const;

	uint2 GetSpriteSize() const;

	const TileSheet* GetTileSheet() const { return tileSheet; }

private:
	TileSheet* tileSheet{ nullptr };
	
	AnimationData animations[MAX_ANIMATIONS];

	uint currentAnimation{ 0 };
	uint currentFrame{ 0 };

	float timeSinceLastFrame{ 0.f };
};
