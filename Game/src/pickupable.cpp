#include "precomp.h"

#include "arrayList.h"
#include "resourceManager.h"

#include "aabb.h"
#include "gameobject.h"

#include "tilemap.h"
#include "level.h"
#include "tilesheet.h"

#include "pickupable.h"



Pickupable::Pickupable(ItemType itemType)
	: GameObject(itemType == ItemType::Pistol ? AABB({ 0.f,0.f }, { 31.f,15.f }) : AABB({ 0.f, 0.f }, {15.f, 15.f}))
	, itemType{itemType}
{
}

void Pickupable::Init(const ResourceManager& resourceManager, Level* _level, irrklang::ISoundEngine* _soundEngine)
{
	GameObject::Init(resourceManager.getTileSheet(TileSheets::Items), _level, _soundEngine);
}


void Pickupable::Draw(Surface& target, int2 offset) const
{
	uint tileIndex = ITEM_TILEINDEX[static_cast<uint>(itemType)];

	

	if (itemType == ItemType::Pistol)
	{
		// gun is special because it is 2 tiles wide
		tileSheet->DrawTile(target, int2(static_cast<int>(position.x), static_cast<int>(position.y)) + level->GetWorldOffset() + offset + int2(tileSheet->GetCellSize().x, 0), tileIndex + 1);
	}
		
	tileSheet->DrawTile(target, int2(static_cast<int>(position.x), static_cast<int>(position.y)) + level->GetWorldOffset() + offset, tileIndex);
}

void Pickupable::DrawDebug(Surface& target, int2 offset) const
{
	GetCollider().draw(target, level->GetWorldOffset() + offset, 0xffff00ff);
}

ItemType Pickupable::PickUp()
{
	// handle inventory stuff
	printf("Pick up item with item Type: %d\n", static_cast<int>(itemType));


	MarkForDelete();
	return itemType;
}
