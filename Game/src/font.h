#pragma once

class Font
{
public:
	Font() = default;
	Font(const char* file, uint2 cellSize, uint ASCIIStart, uint ASCIIEnd);

	void DrawTile(Surface& target, int tile, int2 position) const;
	void DrawCharacter(Surface& target, char chracter, int2 position) const;

	void DrawText(Surface& target, const char* text, int2 position, int2 padding = {0,0}) const;

	int2 GetCharacterSize() const;

private:
	TileSheet charSprites;
	uint asciiStart{ 0 };
	uint asciiEnd{ 0 };
};

