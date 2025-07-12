#include "precomp.h"
#include "tilesheet.h"
#include "font.h"


Font::Font(const char* file, uint2 cellSize, uint ASCIIStart, uint ASCIIEnd)
	: charSprites(file, cellSize)
	, asciiStart{ASCIIStart}
	, asciiEnd {ASCIIEnd}
{
	
}

void Font::DrawTile(Surface& target, int tile, int2 position) const
{
	charSprites.DrawTile(target, position, tile);
}

void Font::DrawCharacter(Surface& target, char character, int2 position) const
{
	if (character >= static_cast<char>(asciiStart) && character <= static_cast<char>(asciiEnd))
	{
		charSprites.DrawTile(target, position, character - asciiStart);
	}
}

void Font::DrawText(Surface& target, const char* text, int2 position, int2 padding) const
{
	char currentChar = text[0];
	int index = 0;

	int x = 0;
	int y = 0;

	while (currentChar != '\0')
	{
		if (currentChar == '\n')
		{
			x = -1;
			y++;
		}
		DrawCharacter(target, currentChar, position + int2(x * (charSprites.GetCellSize().x + padding.x), y * (charSprites.GetCellSize().y + padding.y)));
		currentChar = text[++index];
		x++;
	}
}

int2 Font::GetCharacterSize() const
{
	return int2(static_cast<int>(charSprites.GetCellSize().x),static_cast<int>(charSprites.GetCellSize().y));
}
