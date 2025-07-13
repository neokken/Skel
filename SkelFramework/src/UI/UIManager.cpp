#include "skelpch.h"
#include "UI/UIManager.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui_internal.h>

// ReSharper disable once CppMemberFunctionMayBeStatic
void skel::UIManager::Initialize(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void skel::UIManager::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void skel::UIManager::RegisterPanel(const std::shared_ptr<UIPanel>& panel, const int order)
{
	m_panels.push_back({ .panel= panel, .order= order});
	SortPanels();
}

void skel::UIManager::UnregisterPanel(const std::shared_ptr<UIPanel>& panel)
{
	m_panels.erase(
		std::ranges::remove_if(m_panels, 
		[&](const UIPanelEntry& e) { return e.panel == panel; }).begin(), 
		m_panels.end()
	);
}


void skel::UIManager::SortPanels()
{
	std::ranges::sort(m_panels, [](const UIPanelEntry& a, const UIPanelEntry& b) { return a.order < b.order; });
}


// ReSharper disable once CppMemberFunctionMayBeStatic
void skel::UIManager::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


void skel::UIManager::Render()
{
	SetupDockspace();

	for (const auto& entry : m_panels)
	{
		if (entry.panel && entry.panel->IsEnabled())
		{
			entry.panel->Render();
		}
	}
}


// ReSharper disable once CppMemberFunctionMayBeStatic
void skel::UIManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void skel::UIManager::SetupDockspace()
{
	static bool p_open = true;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking; 

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace Root", &p_open, window_flags);
	ImGui::PopStyleVar(3);

	const ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");

	if (!m_dockspaceInitialized)
	{
		m_dockspaceInitialized = true;

		ImGui::DockBuilderRemoveNode(dockspace_id); 
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);

		ImGui::DockBuilderDockWindow("Viewport", dock_main_id);
		ImGui::DockBuilderDockWindow("Console", dock_id_right);

		ImGuiDockNode* viewportNode = ImGui::DockBuilderGetNode(dock_main_id);
		if (viewportNode)
		{
			viewportNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
		}
		
		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoDockingInCentralNode);
	ImGui::End();
}
