#pragma once
#include "UIPanel.h"
#include "GLFW/glfw3.h"


namespace skel
{
	

	class UIManager
	{
	public:
		UIManager() = default;

		void Initialize(GLFWwindow* window);  // pass your GLFW window here to hook ImGui to it
		void Shutdown();

		void RegisterPanel(const std::shared_ptr<UIPanel>& panel, int order = 0);

		void UnregisterPanel(const std::shared_ptr<UIPanel>& panel);


		void BeginFrame();
		void Render();
		void EndFrame();

	private:
		void SetupDockspace();

		void SortPanels();

	private:
		struct UIPanelEntry
		{
			std::shared_ptr<UIPanel> panel;
			int order; // lower = render earlier, higher = render later
		};

		bool m_dockspaceInitialized{ false };
		std::vector<UIPanelEntry> m_panels;
	};

}
