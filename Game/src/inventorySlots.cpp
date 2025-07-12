#include "precomp.h"

#include "inventory.h"
#include "resourceManager.h"

#include "tilesheet.h"
#include "font.h"

#include "inventorySlots.h"

void InventorySlots::Init(const ResourceManager& resourceMananger, Inventory* _inventory)
{
	inventory = _inventory;

	itemSlotIcon = resourceMananger.getTileSheet(TileSheets::ItemSlot);
	itemSheet = resourceMananger.getTileSheet(TileSheets::Items);

	itemCountFont = resourceMananger.getFont(Fonts::ItemAmountNumbers);

}


void InventorySlots::Draw(Surface& target, int2 offset) const
{
	itemSlotIcon->DrawTile(target, offset, 0);
	DrawItem(target, offset, inventory->GetSelectedWeapon());
	
	if (inventory->GetSelectedWeapon() == ItemType::Pistol)
	{
		int ones = inventory->GetAmmoCount() % 10;
		int tens = (inventory->GetAmmoCount() / 10)%10;

		if (tens != 0)
			itemCountFont->DrawTile(target, tens, offset + int2(34, 32));
		if (ones !=0 || tens !=0 )
			itemCountFont->DrawTile(target, ones, offset + int2(34+ 7, 32));
	}

	int2 secondItemSlotOffset = int2(0, itemSlotIcon->GetCellSize().y + 11) + offset;

	itemSlotIcon->DrawTile(target, secondItemSlotOffset, 0);
	DrawItem(target, secondItemSlotOffset, inventory->GetSelectedEquipment());

	

}

void InventorySlots::DrawItem(Surface& target, int2 offset, ItemType item) const
{
	
	if (item == ItemType::Pistol)
	{
		itemSheet->DrawTile(target, offset+ int2(11, 11), static_cast<int>(item));
		itemSheet->DrawTile(target, offset+int2(11+itemSheet->GetCellSize().x,11), static_cast<int>(item) + 1);
	}
	else if (item != ItemType::Empty)
	{
		itemSheet->DrawTile(target, offset + int2(16, 11), static_cast<int>(item));
	}
}
