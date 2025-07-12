#pragma once
#include "UIBase.h" // dont kill me :)

class ResourceManager;
class TileSheet;
class Font;

class PortraitFrame : public UIBase
{
public:
	PortraitFrame() = default;

	void Init(const ResourceManager& resourceManager);

	void Update(float deltaTime) override;

	void Draw(Surface& target, int2 offset) const override;

	void SetCharacter(Characters character);

	void SetGlitching(bool value);

public:
	TileSheet* portraits{ nullptr };
	TileSheet* portraitBorder{ nullptr };

	Characters currentCharacter{ Characters::None };

	bool glitching{ false };
	float glitchLine{ 0 };


};

