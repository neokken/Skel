#pragma once

class TileSheet;

template <class>
class ArrayList;

class TileMap
{
public:
	TileMap() = default;
	TileMap(const char* path, uint2 mapSize);
	TileMap(const TileMap& other);
	TileMap(TileMap&& other) noexcept;

	~TileMap();

	TileMap& operator=(const TileMap& rhs);
	TileMap& operator=(TileMap&& rhs) noexcept;

	void Init(TileSheet* tileSheet);


	void DrawTileMap(Surface& target, int2 drawPosition) const;
	void DrawTileMap(Surface& target, int2 drawPosition, int2 topLeft, int2 bottomRight) const;

	void DrawTile(Surface& target, int2 drawPosition, int2 tilePosition) const;
	void DrawHalfTile(Surface& target, int2 drawPosition, int2 tilePosition) const;

	const uint2& GetMapSize() const { return mapSize; }

	uint GetTile(int2 tilePosition) const;
	uint GetTile(uint tilePosition) const;

	void SetTile(int2 tilePosition, uint tile);
	void SetTile(uint tilePosition, uint tile);


	bool Raycast(float2 start, float2 end, const ArrayList<uint>& moveableTiles, float2& hitPoint) const;

private:
	void LoadCsv(const char* file);

	uint2 mapSize{ 0 };
	uint* levelData{ nullptr };

	TileSheet* tileSheet{ nullptr }; //not owning
};