#include "precomp.h"

#include "tilesheet.h"
#include "font.h"

#include "resourceManager.h"


ResourceManager::ResourceManager()
{
	tileSheets = new TileSheet[static_cast<int>(TileSheets::Count)];
	fonts = new Font[static_cast<int>(Fonts::Count)];


	for (int i = 0; i < static_cast<int>(TileSheets::Count); i++)
	{
		tileSheets[i] = TileSheet(tileSheetData[i].path, tileSheetData[i].cellSize);
	}

	for (int i = 0; i < static_cast<int>(Fonts::Count); i++)
	{
		fonts[i] = Font(fontData[i].path, fontData[i].cellSize, fontData[i].asciiStart, fontData[i].asciiEnd);
	}

}

ResourceManager::~ResourceManager()
{
	delete[] tileSheets;
	delete[] fonts;
}

Font* ResourceManager::getFont(Fonts font) const
{
	return &fonts[static_cast<int>(font)];
}

TileSheet* ResourceManager::getTileSheet(TileSheets tileSheet) const
{
	return &tileSheets[static_cast<int>(tileSheet)];
}
