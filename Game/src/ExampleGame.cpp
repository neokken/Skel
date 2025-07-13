#include "skelpch.h"
#include "ExampleGame.h"


#include "Math/ColorConverter.h"


skel::EngineInitValues ExampleGame::GetStartupSettings()
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

void ExampleGame::Initialize()
{
	const skel::Renderer& renderer = skel::Engine::GetInstance().GetRenderer();

	m_screen = std::make_unique<skel::Surface>(renderer.GetWidth(), renderer.GetHeight(),true);

	m_testImage = std::make_unique<skel::Surface>("assets/testAsset.png", false);
	ballSize = { m_testImage->GetWidth(), m_testImage->GetHeight() };
}

void ExampleGame::Update(const float deltaTime)
{
	const auto& input = skel::Engine::GetInstance().GetInput();

	if (input.IsKeyDown(GLFW_KEY_RIGHT)) ballVel.x += 200.f * deltaTime;
	if (input.IsKeyDown(GLFW_KEY_LEFT))  ballVel.x -= 200.f * deltaTime;
	if (input.IsKeyDown(GLFW_KEY_UP))    ballVel.y -= 200.f * deltaTime;
	if (input.IsKeyDown(GLFW_KEY_DOWN))  ballVel.y += 200.f * deltaTime;

	ballPos += ballVel * deltaTime;

	if (ballPos.x < 0 || ballPos.x + ballSize.x > m_screen->GetWidth())
	{
		ballVel.x = -ballVel.x;
		ballPos.x = std::clamp(ballPos.x, 0.f, static_cast<float>(m_screen->GetWidth() - ballSize.x));
	}
	if (ballPos.y < 0 || ballPos.y + ballSize.y > m_screen->GetHeight())
	{
		ballVel.y = -ballVel.y;
		ballPos.y = std::clamp(ballPos.y, 0.f, static_cast<float>(m_screen->GetHeight() - ballSize.y));
	}

	constexpr float friction = 0.5f;         
	ballVel -= ballVel * friction * deltaTime;   


	m_totalTime += deltaTime;


}

void ExampleGame::Render(skel::Renderer& renderer)
{
	// Shift background hue each second
	skel::float3 bg = skel::HSVtoRGB(fmodf(m_totalTime * 0.1f, 1.f), .7f, .7f);
	m_screen->Clear(skel::ColorToUint32(skel::float4(bg, 1.f)));

	// Draw sprite
	m_testImage->CopyTo({ int(ballPos.x), int(ballPos.y) }, m_screen.get());


	renderer.BlitSurface(m_screen.get(), 0, 0);
}
