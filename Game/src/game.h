// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#pragma once
#include "Core/GameBase.h"
#include "resourceManager.h"


class MetalGear;
class Player;
class TileSheet;




class MetalGearGame : public skel::GameBase
{
public:
	MetalGearGame() = default;
	~MetalGearGame();

	skel::EngineInitValues GetStartupSettings() override;

	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(skel::Renderer& renderer) override;

private:
	std::unique_ptr<skel::Surface> m_screen{ nullptr };

	ResourceManager resourceManager;
	MetalGear* metalGearGame{ nullptr };
	Player* playerPointer{ nullptr };



	irrklang::ISoundEngine* soundEngine{ nullptr };

};



