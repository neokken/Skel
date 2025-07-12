#include "skelpch.h"
#include "VoxelSpaceGame.h"


#include "Math/ColorConverter.h"


skel::EngineInitValues VoxelSpaceGame::GetStartupSettings()
{
	return
	{
		.windowTitle = "Example Game",
		.startupWindowWidth = 1600,
		.startupWindowHeight = 900,

		.rendererWidth = 288,
		.rendererHeight = 220,

		.vsyncEnabled = false,

		.showStatsPanelOnStartup = false,

	};
}

void VoxelSpaceGame::Initialize()
{
	const skel::Renderer& renderer = skel::Engine::GetInstance().GetRenderer();

	m_screen = std::make_unique<skel::Surface>(renderer.GetWidth(), renderer.GetHeight(),true);

	m_testImage = std::make_unique<skel::Surface>("assets/testAsset.png", false);


	const skel::float3 color = skel::HSVtoRGB(fmod(m_totalTime * .1f, 1.f), .7f, .7f);
	m_screen->Clear(skel::ColorToUint32(skel::float4(color, 1.f)));
	m_clearColor = skel::ColorToUint32(skel::float4(color, 1.f));

	m_lastMousePosition = skel::Engine::GetInstance().GetInput().GetMousePosition();
}

void VoxelSpaceGame::Update(const float deltaTime)
{
	m_totalTime += deltaTime;

}

void VoxelSpaceGame::Render(skel::Renderer& renderer)
{
	const skel::InputManager& input = skel::Engine::GetInstance().GetInput();
	skel::int2 position = input.GetMousePosition();
	if (input.IsMouseDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		m_screen->Line(m_lastMousePosition, position, 0xffffffff);
		//m_screen->Plot(position.x, position.y, 0xffffffff);
	}
	if (input.IsMouseDown(GLFW_MOUSE_BUTTON_RIGHT)) m_screen->Plot(position.x, position.y, m_clearColor);


	const skel::float3 color = skel::HSVtoRGB(fmodf(m_totalTime * .1f, 1.f), .7f, .7f);
	m_screen->Clear(skel::ColorToUint32(skel::float4(color, 1.f)));
	m_clearColor = skel::ColorToUint32(skel::float4(color, 1.f));


	const float imagexpos = (-std::cosf(m_totalTime * .2f) * .5f + .5f) *172 - static_cast<float>(m_testImage->GetWidth()) - 1.f;

	m_testImage->CopyTo({ static_cast<int>(imagexpos),50}, m_screen.get());

	m_lastMousePosition = position;
	renderer.BlitSurface(m_screen.get(), 0, 0);
}
