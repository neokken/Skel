#pragma once
#include "Engine.h"
#include "Renderer/Renderer.h"

namespace skel
{

	class GameBase
	{
	public:
		GameBase() = default;
		virtual ~GameBase() = default;


		virtual EngineInitValues GetStartupSettings() { return {}; } // default values 

		virtual void Initialize() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render(Renderer& renderer) = 0;
	};

}

