#include "precomp.h"
#include "tilesheet.h"

#include "resourceManager.h"
#include "portraitFrame.h"

void PortraitFrame::Init(const ResourceManager& resourceManager)
{
	portraits = resourceManager.getTileSheet(TileSheets::Portraits);
	portraitBorder = resourceManager.getTileSheet(TileSheets::PortraitBorder);

}

void PortraitFrame::Update(float deltaTime)
{

	if (glitching)
	{
		float glitchLineSpeed = 20.f; // pixels per second

		glitchLine += glitchLineSpeed * deltaTime;

		if (glitchLine >= static_cast<float>(portraits->GetCellSize().y))
		{
			glitchLine = 0;
		}
	}
}

void PortraitFrame::Draw(Surface& target, int2 offset) const
{
	portraitBorder->DrawTile(target, offset, 0);

	portraits->DrawTile(target, offset + int2(2, 2), static_cast<int>(currentCharacter));

	if (glitching)
	{
		target.Line(offset.x+2, offset.y+2+ static_cast<int>(glitchLine), offset.x+2+portraits->GetCellSize().x, offset.y + 2+ static_cast<int>(glitchLine), 0); // need to do the glitch effect

	}
}

void PortraitFrame::SetCharacter(Characters character)
{
	currentCharacter = character;
}

void PortraitFrame::SetGlitching(bool value)
{
	glitching = value;
}
