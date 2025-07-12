#pragma once

namespace skel
{
	class InputManager;
}

class Inventory;
class ResourceManager;
class Font;


class InventoryScreen : UIBase
{
public:
	InventoryScreen() = default;
	
	void Init(const ResourceManager& resourceManager, Inventory* inventory, bool isWeaponView);

	void Update(float deltaTime) override;
	void Draw(Surface& target, int2 offset = { 0,0 }) const override;


	void SetWeaponView(bool value);

private:
	
	void DrawInventorySlots(Surface& target, const int2& offset) const;
	Inventory* inventory{ nullptr };
	skel::InputManager* inputManager{ nullptr };
	

	bool isWeaponView{ true };
	
	Font* font{ nullptr };
	TileSheet* itemSprites{ nullptr };

	TextBox description;
	//




};
