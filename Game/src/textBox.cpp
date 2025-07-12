#include "precomp.h"
#include "resourceManager.h"
#include "tilesheet.h"
#include "font.h"
#include "textBox.h"

void TextBox::Init(const ResourceManager& resourceManager)
{
	font = resourceManager.getFont(Fonts::MainFont);
}


void TextBox::Draw(Surface& target, int2 offset) const
{
	DrawBox(target, offset);
	DrawText(target, offset);
}

void TextBox::SetText(const char* _text)
{
	text = _text;
}

void TextBox::SetSize(int2 _size)
{
	size = _size;
}

const Font* TextBox::GetFont() const
{
	return font;
}


void TextBox::DrawBox(Surface& target, int2 offset) const
{

	if (size.x >= 1 && size.y >= 1)
	{
		target.Bar(offset.x, offset.y, offset.x + size.x - 1, offset.y + size.y - 1, 0xffd83700);
		target.Box(offset.x, offset.y, offset.x + size.x - 1, offset.y + size.y - 1, 0xffffffff);
	}

}

void TextBox::DrawText(Surface& target, int2 offset) const
{
	font->DrawText(target, text, offset + int2(6, 6), int2(0,1));
}
