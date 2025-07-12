#pragma once


class TileSheet;
class AnimatedSprite;

class ObjectPool;
class Level;
class Player;

class ResourceManager;


class GameObject
{
public:
	GameObject() = default;
	GameObject(const AABB& aabb);

	void Init(TileSheet* tileSheet, Level* level, irrklang::ISoundEngine* soundEngine);
	void Init(const ResourceManager& resourceManager, Level* level, irrklang::ISoundEngine* soundEngine);
	virtual ~GameObject() = default;

	
	virtual void Update(float deltaTime) = 0;

	virtual void Draw(Surface& target, int2 offset = { 0,0 }) const;
	virtual void DrawDebug(Surface& target, int2 offset = { 0,0 }) const = 0;

	virtual void SetPosition(float2 newPos);
	const float2& GetPosition() const { return position; }

	bool IsDisabled() const { return disabled; }
	void SetDisabled(bool value) { disabled = value; }

	const AABB& GetCollider() const { return collider; }

	bool PixelCollision(const GameObject& other) const;

	// if the gameobject in the current room view
	bool IsVisible() const;

	virtual uint GetCurrentTileIndex() const;

	void MarkForDelete();
	bool IsMarkedForDelete() const;
	
	virtual int GetSortingOffset() const { return 0; }

protected:
	float2 position{ 0.f, 0.f};
	AABB collider{ float2{0.f,0.f},float2{ 0.f,0.f} };

	TileSheet* tileSheet{ nullptr };

	Level* level{ nullptr };

	irrklang::ISoundEngine* soundEngine{ nullptr };

private:
	bool markedForDelete{ false };
	bool disabled{ false }; // disables the update and draw function

	
};

