#include "precomp.h"
#include "animatedTextBox.h"


void AnimatedTextBox::Init(const ResourceManager& resourceManager)
{
	TextBox::Init(resourceManager);
}

void AnimatedTextBox::Update(float deltaTime)
{
	if (progress >= 1.f)
		return;

	if (progressSpeed <= 0.f)
	{
		progress = 1.f;
	}
	else
	{
		progress += deltaTime * progressSpeed;
	}
	progress = std::min(progress, 1.f);

	float2 currentTopLeft = lerp(float2(static_cast<float>(startTopLeft.x), static_cast<float>(startTopLeft.y)), float2(static_cast<float>(targetTopLeft.x), static_cast<float>(targetTopLeft.y)), progress);

	float2 currentBottomRight= lerp(float2(static_cast<float>(startBottomRight.x), static_cast<float>(startBottomRight.y)), float2(static_cast<float>(targetBottomRight.x), static_cast<float>(targetBottomRight.y)), progress);

	TextBox::SetSize(int2(static_cast<int>(currentBottomRight.x - currentTopLeft.x),
		static_cast<int>(currentBottomRight.y - currentTopLeft.y)));

	positionOffset = int2(static_cast<int>(currentTopLeft.x),static_cast<int>(currentTopLeft.y));
}

void AnimatedTextBox::Draw(Surface& target, int2 offset) const
{
	TextBox::DrawBox(target, offset + positionOffset);
	if (progress >= 1.f)
		TextBox::DrawText(target, offset + positionOffset);
}

void AnimatedTextBox::SetTarget(const int2& topLeft, const int2& bottomRight, float time)
{
	startTopLeft = targetTopLeft;
	startBottomRight = targetBottomRight;

	targetTopLeft = topLeft;
	targetBottomRight = bottomRight;

	progressSpeed = 1.f/time;
	progress = 0;
}

void AnimatedTextBox::SetText(char* _text)
{
	TextBox::SetText(_text);
}

const Font* AnimatedTextBox::GetFont() const
{
	return TextBox::GetFont();
}

float AnimatedTextBox::GetProgress() const
{
	return progress;
}

bool AnimatedTextBox::Finished() const
{
	return progress >= 1.f;
}
