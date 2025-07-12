#pragma once
#include "UIBase.h"

class Player;

class HealthBar : public UIBase
{
public:
    HealthBar() = default;
    void Init(const ResourceManager& resourceManager, Player* player);

    void Update(float deltaTime);

    void Draw(Surface& target, int2 offset = { 0, 0 }) const;

private:
    Player* player{nullptr};
    
    TileSheet* sprite;
};

