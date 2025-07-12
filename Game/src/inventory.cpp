#include "precomp.h"
#include "inventory.h"


Inventory::Inventory()
{
	for (int i = 0; i < 14; i++)
	{
		weaponList[i] = ItemType::Empty;
		equipmentList[i] = ItemType::Empty;
	}
}

void Inventory::HandlePickupable(ItemType item)
{

	if (item == ItemType::Cigar || item == ItemType::Ration)
	{
		uint i = GetFirstEmptySlot(equipmentList);
		if (i != -1)
		{
			equipmentList[i] = item;
		}
	}
	else if (item == ItemType::Ammo)
	{
		ammoCount += 30;
	}
	else if (item == ItemType::Pistol)
	{
		uint i = GetFirstEmptySlot(weaponList);
		if (i != -1)
		{
			weaponList[i] = item;
		}
		ammoCount += 30;
	}
}

ItemType Inventory::GetSelectedWeapon() const
{
	if (selectedWeaponIndex == -1)
		return ItemType::Empty;

	return weaponList[selectedWeaponIndex];
}

ItemType Inventory::GetSelectedEquipment() const
{
	if (selectedEquipmentIndex == -1)
		return ItemType::Empty;

	return equipmentList[selectedEquipmentIndex];
}

void Inventory::UseSelectedWeapon()
{
	if (GetSelectedWeapon() == ItemType::Pistol)
	{
		ammoCount--;
	}
}

void Inventory::UseSelectedEquipment()
{
	if (GetSelectedEquipment() == ItemType::Ration)
	{
		equipmentList[selectedEquipmentIndex] = ItemType::Empty;
	}
}

const ItemType* Inventory::GetWeaponList() const
{
	return weaponList;
}

const ItemType* Inventory::GetEquipmentList() const
{
	return equipmentList;
}

uint Inventory::GetWeaponSelectIndex() const
{
	return selectedWeaponIndex;
}

uint Inventory::GetEquipmentSelectIndex() const
{
	return selectedEquipmentIndex;
}

uint Inventory::GetAmmoCount() const
{
	return ammoCount;
}

void Inventory::ClearItemLists()
{
	for (int i = 0; i < INVENTORY_SLOT_COUNT; i++)
	{
		weaponList[i] = ItemType::Empty;
		equipmentList[i] = ItemType::Empty;
	}
}

void Inventory::SetWeaponSelectedIndex(int value)
{
	if (value < 0 || value >= 14)
		return;


	selectedWeaponIndex = value;
	
}

void Inventory::SetEquipmentSelectedIndex(int value)
{
	if (value < 0 || value >= 14)
		return;


	selectedEquipmentIndex = value;

}

void Inventory::SetAmmoCount(int value)
{
	ammoCount = value;
}


void Inventory::RemoveItem(ItemType* itemArray, int index)
{
	itemArray[index] = ItemType::Empty;
}


int Inventory::GetFirstEmptySlot(ItemType* array)
{
	for (uint i = 0; i < 14; i++)
	{
		if (array[i] == ItemType::Empty)
			return i;
	}
	return -1;
}

