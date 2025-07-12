#pragma once

class Player;


class Inventory
{
public:
	Inventory();

	void HandlePickupable(ItemType item);
	

	ItemType GetSelectedWeapon() const;
	ItemType GetSelectedEquipment() const;

	void UseSelectedWeapon();
	void UseSelectedEquipment();

	const ItemType* GetWeaponList() const;
	const ItemType* GetEquipmentList() const;

	uint GetWeaponSelectIndex() const;
	uint GetEquipmentSelectIndex() const;

	uint GetAmmoCount() const;


	void ClearItemLists();

	void SetWeaponSelectedIndex(int value);
	void SetEquipmentSelectedIndex(int value);
	
	void SetAmmoCount(int value);

	void RemoveItem(ItemType* itemArray, int index);

private:

	int GetFirstEmptySlot(ItemType* array); // retunrs negative 1
	

	
	ItemType weaponList[INVENTORY_SLOT_COUNT] { ItemType::Empty };
	ItemType equipmentList[INVENTORY_SLOT_COUNT]{ ItemType::Empty };

	int selectedWeaponIndex{ -1 };
	int selectedEquipmentIndex{ -1 };
	
	uint ammoCount = 0;
};