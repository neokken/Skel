#pragma once

class UIBase
{
public:
	UIBase() {}
	
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(Surface& target, int2 offset = {0,0}) const = 0;
};