#pragma once
#include "Core/GameBase.h"
#include "Renderer/Surface.h"

class VoxelSpaceGame : public skel::GameBase
{
public:
	VoxelSpaceGame() = default;

	skel::EngineInitValues GetStartupSettings() override;

	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(skel::Renderer& renderer) override;

private:
	float m_totalTime{ 0.f };

	uint m_clearColor;

	std::unique_ptr<skel::Surface> m_screen{ nullptr };

};

