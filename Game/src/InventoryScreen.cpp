#include "precomp.h"

#include "resourceManager.h"

#include "inventory.h"

#include "tilesheet.h"
#include "font.h"
#include "UIBase.h"

#include "textBox.h"

#include "InventoryScreen.h"

#include "Core/Engine.h"

void InventoryScreen::Init(const ResourceManager& resourceManager, Inventory* _inventory, bool _isWeaponView)
{
	font = resourceManager.getFont(Fonts::MainFont);
	inventory = _inventory;
	inputManager = &Engine::GetInstance().GetInput();
	isWeaponView = _isWeaponView;

	itemSprites = resourceManager.getTileSheet(TileSheets::Items);

	description.Init(resourceManager);
	description.SetSize(int2(TERRAIN_TILE_SIZE * ROOM_WIDTH, 44));
}


void InventoryScreen::Update(float)
{
	ItemType item = isWeaponView ? inventory->GetSelectedWeapon() : inventory->GetSelectedEquipment();

	description.SetText(ITEM_DESCRIPTIONS[static_cast<int>(item)]);


	int selectedSlot = isWeaponView ? inventory->GetWeaponSelectIndex() : inventory->GetEquipmentSelectIndex();
	int2 selectionPos = int2(selectedSlot / 7, selectedSlot % 7);

	if (inputManager->IsKeyJustDown(GLFW_KEY_DOWN))
	{
		if (selectionPos.y < 6)
		{
			selectedSlot++;
		}
	}
	else if (inputManager->IsKeyJustDown(GLFW_KEY_UP))
	{
		if (selectionPos.y > 0)
		{
			selectedSlot--;
		}
	}
	else if (inputManager->IsKeyJustDown(GLFW_KEY_LEFT))
	{
		if (selectionPos.x > 0)
		{
			selectedSlot-=7;
		}
	}
	else if (inputManager->IsKeyJustDown(GLFW_KEY_RIGHT))
	{
		if (selectionPos.x < 2)
		{
			selectedSlot+=7;
		}
	}

	if (isWeaponView)
		inventory->SetWeaponSelectedIndex(selectedSlot);
	else
		inventory->SetEquipmentSelectedIndex(selectedSlot);

}

void InventoryScreen::Draw(Surface& target, int2 offset) const
{
	target.Bar(offset.x, offset.y, offset.x + ROOM_WIDTH * TERRAIN_TILE_SIZE, offset.y + ROOM_HEIGHT * TERRAIN_TILE_SIZE, 0);


	const char* text = isWeaponView ? "WEAPON SELECT" : "EQUIPMENT SELECT";
	
	int centerText = (ROOM_WIDTH * TERRAIN_TILE_SIZE - (static_cast<int>(strlen(text)) * 8))/2;
	
	font->DrawText(target, text, offset + int2(centerText, 8));

	description.Draw(target, offset + int2(0, ROOM_HEIGHT*TERRAIN_TILE_SIZE - 44));


	// draw Items

	DrawInventorySlots(target, offset+int2(0, 20));

}

void InventoryScreen::DrawInventorySlots(Surface& target, const int2& offset) const
{
	const ItemType* items = isWeaponView ? inventory->GetWeaponList() : inventory->GetEquipmentList();
	int selectedSlot = isWeaponView ? inventory->GetWeaponSelectIndex() : inventory->GetEquipmentSelectIndex();


	for (int i = 0; i < 14; i++)
	{
		int x = (i / 7 ) * (itemSprites->GetCellSize().x*3)+ itemSprites->GetCellSize().x;
		int y = (i % 7) * (itemSprites->GetCellSize().y);

		if (selectedSlot == i)
		{
			font->DrawText(target, ">", offset + int2(x- itemSprites->GetCellSize().x/2, y+4));
		}

		
		if (items[i] == ItemType::Pistol)
		{
			itemSprites->DrawTile(target, offset + int2(x, y), static_cast<int>(items[i]));
			itemSprites->DrawTile(target, offset + int2(x + itemSprites->GetCellSize().x, y), static_cast<int>(items[i]) + 1);
		}
		else if (items[i] != ItemType::Empty)
		{
			itemSprites->DrawTile(target, offset + int2(x, y), static_cast<int>(items[i]));
		}


	}
}

void InventoryScreen::SetWeaponView(bool value)
{
	isWeaponView = value;
}
