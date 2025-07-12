#pragma once


class TileSheet;
class Font;


class ResourceManager
{
public:
	ResourceManager(); // loads all files based on what is in common.h
	~ResourceManager();


	Font* getFont(Fonts font) const;
	TileSheet* getTileSheet(TileSheets tileSheet) const;

private:
	TileSheet* tileSheets;
	Font* fonts;
};

