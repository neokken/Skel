#pragma once
#include "UIBase.h"


class ResourceManager;
class MetalGear;

class SaveLoadScreen : public UIBase
{
public:
	SaveLoadScreen() = default;

	void Init(const ResourceManager& resourceManager,  MetalGear* game);

	void Update(float deltaTime) override;

	void Draw(Surface& target, int2 offset = { 0,0 }) const override;

private:
	MetalGear* game{ nullptr };
	skel::InputManager* inputManager{ nullptr };
	TextBox textbox;

	int currentOption{ 0 };
	int optionAmount{ 2 };

};

