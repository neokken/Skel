#pragma once



class TileSheet
{
public:
	TileSheet() = default;
	TileSheet(const char* file, uint2 cellSize);

	TileSheet(const TileSheet& other);

	~TileSheet();
	
	TileSheet& operator=(const TileSheet& rhs);


	void DrawTile(Surface& target, int2 targetPosition, uint cellPosition) const;
	void DrawTile(Surface& target, int2 targetPosition, uint2 cellPosition) const;

	void DrawTopHalfTile(Surface& target, int2 targetPosition, uint cellPosition) const;

	const uint2& GetCellSize() const { return cellSize; }

	const Surface* GetSurface() const { return surface; }

	void SetColorMask(uint color);
	uint GetColorMask() const;

	uint GetPixel(uint tileIndex, int2 position) const;

	uint2 GetTileSize() const { return cellSize; }

	bool DoPixelOverlap(uint tileIndex, int2 position, const TileSheet& other, uint otherTileIndex, int2 otherPosition) const;

private:
	Surface* surface{ nullptr };

	uint colorMask = DEFAULT_MASK_COLOR;

	uint* cellPositionStart{ nullptr };

	uint2 cellSize{ 0 };
	uint2 cellGrid{ 0 };

};