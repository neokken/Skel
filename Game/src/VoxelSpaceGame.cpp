#include "skelpch.h"
#include "VoxelSpaceGame.h"


#include "Math/ColorConverter.h"


skel::EngineInitValues VoxelSpaceGame::GetStartupSettings()
{
	return
	{
		.startupWindowWidth= 1600,
		.startupWindowHeight= 900,
		.rendererWidth= 1600/10,
		.rendererHeight= 900/10,
		.vsyncEnabled = false
	};
}

void VoxelSpaceGame::Initialize()
{
	const skel::Renderer& renderer = skel::Engine::GetInstance().GetRenderer();

	m_screen = std::make_unique<skel::Surface>(renderer.GetWidth(), renderer.GetHeight());

	const skel::float3 color = skel::HSVtoRGB(fmod(m_totalTime * .1f, 1.f), .7f, .7f);
	m_screen->Clear(skel::ColorToUint32(skel::float4(color, 1.f)));
	m_clearColor = skel::ColorToUint32(skel::float4(color, 1.f));
}

void VoxelSpaceGame::Update(const float deltaTime)
{
	m_totalTime += deltaTime;
	//SKEL_INFO("FPS : {} ({}ms)", 1.f / deltaTime, deltaTime*1000);


}

void VoxelSpaceGame::Render(skel::Renderer& renderer)
{
	const skel::InputManager& input = skel::Engine::GetInstance().GetInput();
	skel::int2 position = input.GetMousePosition();
	if (input.IsMouseDown(GLFW_MOUSE_BUTTON_LEFT)) m_screen->Plot(position.x, position.y, 0xffffffff);
	if (input.IsMouseDown(GLFW_MOUSE_BUTTON_RIGHT)) m_screen->Plot(position.x, position.y, m_clearColor);

	if (input.IsKeyDown(GLFW_KEY_C))
	{
		const skel::float3 color = skel::HSVtoRGB(fmod(m_totalTime * .1f, 1.f), .7f, .7f);
		m_screen->Clear(skel::ColorToUint32(skel::float4(color, 1.f)));
		m_clearColor = skel::ColorToUint32(skel::float4(color, 1.f));
	}


	renderer.BlitSurface(m_screen.get(), 0, 0);
}
