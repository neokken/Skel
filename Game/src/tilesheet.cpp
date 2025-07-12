#include "precomp.h"
#include "tilesheet.h"

TileSheet::TileSheet(const char* file, uint2 cellSize)
	: surface { new Surface(std::string(file), false)}
	, cellSize { cellSize}
{
	
	cellGrid.x = surface->GetWidth() / cellSize.x;
	cellGrid.y = surface->GetHeight() / cellSize.y;


	cellPositionStart = new uint[cellGrid.x * cellGrid.y];

	for (uint y = 0; y < cellGrid.y; y++)
	{
		for (uint x = 0; x < cellGrid.x; x++)
		{
			cellPositionStart[y * cellGrid.x + x] = 
				x * cellSize.x + y* cellSize.y * surface->GetWidth();

		}
	}

}

TileSheet::TileSheet(const TileSheet& other)
{
	cellSize = other.cellSize;
	cellGrid = other.cellGrid;
	colorMask = other.colorMask;

	surface = new Surface(other.surface->GetWidth(), other.surface->GetHeight(), false);
	other.surface->CopyTo(0,0,surface);

	cellPositionStart = new uint[cellGrid.x * cellGrid.y];
	for (uint i = 0; i < cellGrid.x*cellGrid.y; i++)
	{
		cellPositionStart[i] = other.cellPositionStart[i];

	}
}

TileSheet::~TileSheet()
{
	delete surface;
	delete[] cellPositionStart;
}

TileSheet& TileSheet::operator=(const TileSheet& rhs)
{
	if (this != &rhs)
	{
		cellSize = rhs.cellSize;
		cellGrid = rhs.cellGrid;
		colorMask = rhs.colorMask;

		delete surface;
		delete[] cellPositionStart;

		surface = new Surface(rhs.surface->GetWidth(), rhs.surface->GetHeight(), false);
		rhs.surface->CopyTo(0, 0, surface);

		cellPositionStart = new uint[cellGrid.x * cellGrid.y];
		for (uint i = 0; i < cellGrid.x * cellGrid.y; i++)
		{
			cellPositionStart[i] = rhs.cellPositionStart[i];
		}
	}
	return *this;
}

void TileSheet::DrawTile(Surface& target, int2 targetPosition, uint cellPosition) const
{

	if (cellPosition > cellGrid.x*cellGrid.y)
		assert(0); // CellPosition is not valid!

	uint startBufferPosition = cellPositionStart[cellPosition];

	int clipXLeftOffset =  std::max(0, targetPosition.x) - targetPosition.x;
	int clipXRightOffset = std::min(targetPosition.x + static_cast<int>(cellSize.x), static_cast<int>(target.GetWidth())) - targetPosition.x - static_cast<int>(cellSize.x);

	int clipYTopOffset =  std::max(0, targetPosition.y) - targetPosition.y;
	int clipYBottomOffset = std::min(targetPosition.y + static_cast<int>(cellSize.y), 
		static_cast<int>(target.GetHeight())) - targetPosition.y - static_cast<int>(cellSize.y);


	int drawWidth = -clipXLeftOffset + cellSize.x + clipXRightOffset;
	int drawHeight = -clipYTopOffset + cellSize.y + clipYBottomOffset;


	for (int y = 0; y < drawHeight; y++)			// If we don't need transparency we could use memcpy i
	{
		for (int x = 0; x < drawWidth; x++)
		{
			int targetSurfaceOffset = (targetPosition.y + y + clipYTopOffset) * target.GetWidth() + targetPosition.x + x + clipXLeftOffset;
			int ownSurfaceOffset = (y + clipYTopOffset) * surface->GetWidth() + x + clipXLeftOffset;
			

			uint color = surface->GetBuffer()[startBufferPosition + ownSurfaceOffset];


			if (colorMask!= color)
				target.GetBuffer()[targetSurfaceOffset] = color;
		}
	}

	target.MarkAsDirty();
}

void TileSheet::DrawTile(Surface& target, int2 targetPosition, uint2 cellPosition) const
{

	DrawTile(target, targetPosition, cellPosition.y*cellGrid.x+ cellPosition.x);

	
}

void TileSheet::DrawTopHalfTile(Surface& target, int2 targetPosition, uint cellPosition) const
{
	if (cellPosition > cellGrid.x * cellGrid.y)
		assert(0); // CellPosition is not valid!

	uint startBufferPosition = cellPositionStart[cellPosition];

	int clipXLeftOffset = std::max(0, targetPosition.x) - targetPosition.x;
	int clipXRightOffset = std::min(targetPosition.x + static_cast<int>(cellSize.x),
		static_cast<int>(target.GetWidth())) - targetPosition.x - static_cast<int>(cellSize.x);

	int clipYTopOffset = std::max(0, targetPosition.y) - targetPosition.y;
	int clipYBottomOffset = std::min(targetPosition.y + static_cast<int>(cellSize.y),
		static_cast<int>(target.GetHeight())) - targetPosition.y - static_cast<int>(cellSize.y);


	int drawWidth = -clipXLeftOffset + cellSize.x + clipXRightOffset;
	int drawHeight = -clipYTopOffset + cellSize.y + clipYBottomOffset;


	for (int y = 0; y < drawHeight/2; y++)			// If we don't need transparency we could use memcpy i
	{
		for (int x = 0; x < drawWidth; x++)
		{
			int targetSurfaceOffset = (targetPosition.y + y + clipYTopOffset) * target.GetWidth() + targetPosition.x + x + clipXLeftOffset;
			int ownSurfaceOffset = (y + clipYTopOffset) * surface->GetWidth() + x + clipXLeftOffset;


			uint color = surface->GetBuffer()[startBufferPosition + ownSurfaceOffset];

			if (colorMask != color)
				target.GetBuffer()[targetSurfaceOffset] = color;
		}
	}
	target.MarkAsDirty();
}

void TileSheet::SetColorMask(uint color)
{
	colorMask = color;
}

uint TileSheet::GetColorMask() const
{
	return colorMask;
}

uint TileSheet::GetPixel(uint tileIndex, int2 position) const
{
	if (position.x < 0 || position.x >= static_cast<int>(cellSize.x) ||
		position.y < 0 || position.y >= static_cast<int>(cellSize.y) || tileIndex < 0 || tileIndex >= cellGrid.y*cellGrid.x)
	{
		return colorMask;
	}

	return surface->GetBuffer()[cellPositionStart[tileIndex] + position.y* surface->GetWidth() + position.x];
}

bool TileSheet::DoPixelOverlap(uint tileIndex, int2 position, const TileSheet& other, uint otherTileIndex, int2 otherPosition) const
{

	int2 topleft = int2(std::max(position.x, otherPosition.x), std::max(position.y, otherPosition.y));

	int2 bottomRight = int2(std::min(position.x + cellSize.x, otherPosition.x + other.GetCellSize().x),
		std::min(position.y + cellSize.y, otherPosition.y + other.GetCellSize().y));


	for (int y = topleft.y; y < bottomRight.y; y++)
	{
		for (int x = topleft.x; x < bottomRight.x; x++)
		{
			int2 pos =  int2(x, y) - position;
			int2 otherpos = int2(x, y) - otherPosition;

			uint color = GetPixel(tileIndex, pos);
			uint othercolor = other.GetPixel(otherTileIndex, otherpos);

			if (color != GetColorMask() && othercolor != other.GetColorMask())
			{
				return true;
			}
		}
	}

	return false;
}



