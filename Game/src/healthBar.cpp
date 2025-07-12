#include "precomp.h"

#include "resourceManager.h"

#include "tilesheet.h"

#include "aabb.h"
#include "gameobject.h"
#include "entity.h"
#include "player.h"
#include "healthBar.h"

void HealthBar::Init(const ResourceManager& resourceManager, Player* _player)
{
	sprite = resourceManager.getTileSheet(TileSheets::HealthBar);
	player = _player;
}

void HealthBar::Update(float)
{
}

void HealthBar::Draw(Surface& target, int2 offset) const
{
	sprite->DrawTile(target, offset, 0);

	int maxHealth = player->GetMaxHealth();
	int currentHealth = player->GetCurrentHealth();

	target.Box(5 + offset.x, 7 + offset.y, 5 + offset.x + maxHealth + 2, 11 + offset.y, 0xffffffff);

	target.Bar(6 + offset.x, 8 + offset.y, 6 + offset.x + currentHealth, 10 + offset.y, 0xff0000ff);
}
