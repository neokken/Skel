#include "precomp.h"

#include "resourceManager.h"

#include "aabb.h"
#include "animatedsprite.h"

#include "tilesheet.h"
#include "tilemap.h"
#include "arrayList.h"
#include "level.h"


#include "gameobject.h"


GameObject::GameObject(const AABB& aabb)
	: collider{aabb}
{ 
}

void GameObject::Init(TileSheet* _tileSheet, Level* _level, irrklang::ISoundEngine* _soundEngine)
{
	tileSheet = _tileSheet;
	level = _level;
	soundEngine = _soundEngine;
}

void GameObject::Init(const ResourceManager& resourceManager, Level* _level, irrklang::ISoundEngine* _soundEngine)
{
	Init(resourceManager.getTileSheet(TileSheets::Empty), _level, _soundEngine);
}

void GameObject::Draw(Surface& target, int2 offset) const
{
	if (disabled || !IsVisible())
		return;

	int2 localPosition = level->worldToScreenSpace(position);
	tileSheet->DrawTile(target, localPosition + offset, 0);
}

void GameObject::SetPosition(float2 newPosition)
{
	float2 offset = newPosition - position;
	position = newPosition;
	collider.setPosition(collider.topLeft + offset);
}

bool GameObject::PixelCollision(const GameObject& other) const
{	
	const TileSheet* otherTileSheet = other.tileSheet;

	int2 pos = int2(static_cast<int>(position.x), static_cast<int>(position.y));
	int2 otherpos = int2(static_cast<int>(other.position.x), static_cast<int>(other.position.y));

	uint tileIndex = GetCurrentTileIndex();
	uint othertileIndex = other.GetCurrentTileIndex();

	return tileSheet->DoPixelOverlap(tileIndex, pos, *otherTileSheet, othertileIndex, otherpos);
}

bool GameObject::IsVisible() const
{
	int2 temp = level->worldToScreenSpace(position);

	float2 screenPosition = float2{static_cast<float>(temp.x),static_cast<float>(temp.y)};

	return screenPosition.x <= static_cast<float>(ROOM_WIDTH * TERRAIN_TILE_SIZE) && screenPosition.x + static_cast<float>(tileSheet->GetCellSize().x) >= 0.f &&
		screenPosition.y <= static_cast<float>(ROOM_HEIGHT * TERRAIN_TILE_SIZE) && screenPosition.y + static_cast<float>(tileSheet->GetCellSize().y) >= 0.f;
}

uint GameObject::GetCurrentTileIndex() const
{
	return uint(0);
}

void GameObject::MarkForDelete()
{
	markedForDelete = true;
}

bool GameObject::IsMarkedForDelete() const
{
	return markedForDelete;
}
