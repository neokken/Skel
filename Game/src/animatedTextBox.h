#pragma once

#include "UIBase.h" // >:) hehe
#include "textBox.h"

class ResourceManager;

class AnimatedTextBox : private TextBox
{
public:
	AnimatedTextBox() = default;

	void Init(const ResourceManager& resourceManager);

	void Update(float deltaTime) override;
	void Draw(Surface& target, int2 offset) const override;

	void SetTarget(const int2& topLeft, const int2& bottomRight, float time);
	void SetText(char* text);

	const Font* GetFont() const;

	float GetProgress() const;
	bool Finished() const;
private:
	int2 startTopLeft{ 0 };
	int2 startBottomRight{ 0 };

	int2 targetBottomRight{ 0 };
	int2 targetTopLeft{ 0 };

	int2 positionOffset{ 0 };

	float progressSpeed = 0.f;

	float progress = 1.f;
};


/*

AnimatexTextBox()




*/
