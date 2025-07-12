#pragma once
#include "UIBase.h" // >:) hehe

class ResourceManager;
class Font;


class TextBox : public UIBase
{
public:
	TextBox() = default;

	void Init(const ResourceManager& resourceManager);


	virtual void Update(float) {}
	virtual void Draw(Surface& target, int2 offset) const override;

	void SetText(const char* text);
	void SetSize(int2 size);

	const Font* GetFont() const;

protected:
	void DrawBox(Surface& target, int2 offset) const;
	void DrawText(Surface& target, int2 offset) const;

	Font* font{nullptr};

	const char* text = "";
	int2 size{ 0 };

};

