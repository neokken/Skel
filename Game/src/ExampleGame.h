#pragma once
#include "Core/GameBase.h"
#include "Renderer/Surface.h"

class ExampleGame : public skel::GameBase
{
public:
	ExampleGame() = default;

	skel::EngineInitValues GetStartupSettings() override;

	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(skel::Renderer& renderer) override;

private:
	std::unique_ptr<skel::Surface> m_screen{ nullptr };
	std::unique_ptr<skel::Surface> m_testImage{ nullptr };

	float m_totalTime = 0;

	
	skel::float2 ballPos{ 100, 100 };
	skel::float2 ballVel{ 60, 45 };
	skel::int2 ballSize;
};

