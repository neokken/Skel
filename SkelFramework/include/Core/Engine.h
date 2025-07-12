#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "InputManager.h"
#include "Renderer/Renderer.h"
#include "UI/ViewportPanel.h"
#include "UI/UIManager.h"
#include "UI/StatPanel.h"


namespace skel
{
	class ConsolePanel;
	class GameBase;

	struct EngineInitValues
	{
		std::string windowTitle{ "Skel App" };

		int startupWindowWidth{ 800 };
		int startupWindowHeight{ 600 };
		//bool startFullscreen{ false }; currently we don't handle fullscreen yet.


		int  rendererWidth{ 800 };
		int  rendererHeight{ 600 };
		bool vsyncEnabled{ false };


		bool showConsolePanelOnStartup{ true };
		bool showStatsPanelOnStartup{ true };

		int consoleToggleKey{ GLFW_KEY_GRAVE_ACCENT };	// set to 0 to disable
	};
	


	class Engine
	{
	public:
		Engine(const Engine&) = delete;
		void operator=(const Engine&) = delete;

		static Engine& GetInstance()
		{
			static Engine instance;
			return instance;
		}


		void Run(GameBase& game);


		// API STUFF

		void CloseApplication() { m_shouldClose = true; }

		float GetNonCampledDeltaTime() const { return m_deltaTimeNonClamped; }

		void EnableVSync(bool enabled);
		bool IsVSync() const { return m_VSyncEnabled; }

		Renderer& GetRenderer() { return *m_renderer; }
		UIManager& GetUIManager() { return m_uiManager; }
		ConsolePanel& GetConsole() { return *m_console; }

		ViewportPanel& GetViewport() { return *m_viewportPanel; }

		const InputManager& GetInput() const { return m_inputManager; }
		InputManager& GetInput() { return m_inputManager; }



		GLFWwindow* GetGLFWWindow() { return m_window; }

		// Callbacks

		void OnFramebufferResize(int width, int height);
	


	private:
		Engine() = default;
		

	private:
		int Initialize();

		int InitializeGLFW();

		void RegisterDefaultCommands();


		void ShutdownGLFW();

		GLFWwindow* m_window{ nullptr };

		int m_windowWidth{800};
		int m_windowHeight{600};
		bool m_VSyncEnabled{false};

		bool m_shouldClose{ false };

		int m_consoleToggleKey{ 0 };

		GameBase* m_game{ nullptr };

		float m_deltaTimeNonClamped{0.f};

		// Systems

		InputManager m_inputManager;

		UIManager m_uiManager;

		Renderer* m_renderer{ nullptr };
		std::shared_ptr<ViewportPanel> m_viewportPanel{nullptr};
		std::shared_ptr<ConsolePanel> m_console{ nullptr };
		std::shared_ptr<StatPanel> m_statsPanel{ nullptr };
	};




}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);