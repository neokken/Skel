#pragma once

class ResourceManager;

class Pickupable : public GameObject
{
public:
	Pickupable(ItemType itemType);

	void Init(const ResourceManager& resourceManager, Level* level, irrklang::ISoundEngine* soundEngine);

	void Update(float) override {}

	void Draw(Surface& target, int2 offset = { 0,0 }) const override;
	void DrawDebug(Surface& target, int2 offset = { 0,0 }) const override;


	ItemType PickUp();

private:
	ItemType itemType{ ItemType::Ammo };


};

