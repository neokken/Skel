// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"


#include "resourceManager.h"

#include "dialogueManager.h"
#include "tilesheet.h"

#include "font.h"

#include "arrayList.h"
#include "pathfinder.h"

#include "aabb.h"
#include "gameobject.h"
#include "entity.h"
#include "player.h"

#include "textBox.h"
#include "enemy.h"


#include "enemySystem.h"
#include "objectPool.h"

#include "tilemap.h"
#include "level.h"

#include "tilesheet.h"
#include "font.h"
#include "UIBase.h"
#include "inventorySlots.h"
#include "radarUI.h"
#include "healthBar.h"
#include "textBox.h"
#include "animatedTextBox.h"
#include "codecTextBox.h"
#include "gameScreen.h"

#include "hud.h"
#include "gameScreen.h"
#include "codec.h"
#include "portraitFrame.h"
#include "codecScreen.h"
#include "InventoryScreen.h"

#include "saveLoadScreen.h"
#include "screenManager.h"

#include "inventory.h"

#include "metalGear.h"
#include "game.h"


MetalGearGame::~MetalGearGame()
{
	delete metalGearGame;
	soundEngine->drop();
}

skel::EngineInitValues MetalGearGame::GetStartupSettings()
{
	return
	{
		.startupWindowWidth = 1600,
		.startupWindowHeight = 900,
		.rendererWidth = GAME_WINDOW_WIDTH,
		.rendererHeight = GAME_WINDOW_HEIGHT,
		.vsyncEnabled = false
	};
}

void MetalGearGame::Initialize()
{
	const skel::Renderer& renderer = skel::Engine::GetInstance().GetRenderer();

	m_screen = std::make_unique<skel::Surface>(renderer.GetWidth(), renderer.GetHeight(), true);

	soundEngine = irrklang::createIrrKlangDevice();
	metalGearGame = new MetalGear();

	metalGearGame->Init(&resourceManager, soundEngine);
	playerPointer = metalGearGame->GetPlayer();


	

}

void MetalGearGame::Update(const float deltaTime)
{
	metalGearGame->Update(deltaTime);


}

void MetalGearGame::Render(skel::Renderer& renderer)
{

	metalGearGame->Draw(*m_screen);



	renderer.BlitSurface(m_screen.get(), 0, 0);
}