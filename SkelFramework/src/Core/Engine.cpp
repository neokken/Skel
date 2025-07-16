#include <skelpch.h>
#include "Core/Engine.h"

#include "UI/ConsolePanel.h"
#include "Core/Timer.h"
#include "Core/Gamebase.h"
#include "imgui_internal.h"


// ReSharper disable once CppMemberFunctionMayBeConst
void skel::Engine::Run(GameBase& game)
{
    m_game = &game;
    Initialize();

    Timer timer;

    while (!glfwWindowShouldClose(m_window) && !m_shouldClose)
    {
        m_deltaTimeNonClamped = timer.elapsed();
        const float deltaTime = std::min(m_deltaTimeNonClamped, 1 / 30.f);
        timer.reset();

        glfwPollEvents();

        // Update game
        if (m_consoleToggleKey!=0 && m_inputManager.IsKeyJustDown(m_consoleToggleKey,true))
        {
            m_console->SetEnabled(!m_console->IsEnabled());
        }



        game.Update(deltaTime);

        m_renderer->Begin();

        game.Render(*m_renderer);

        m_renderer->End();


        m_uiManager.BeginFrame();
        m_uiManager.Render();
        m_uiManager.EndFrame();

        glfwSwapBuffers(m_window);
        m_inputManager.Update(deltaTime);
    }


    ShutdownGLFW();
}

void skel::Engine::EnableVSync(const bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
    m_VSyncEnabled = enabled;        // store for UI / logs
}

void skel::Engine::OnFramebufferResize(const int width, const int height)
{
    m_windowWidth = width;
    m_windowHeight = height;

    glViewport(0, 0, m_windowWidth, m_windowHeight);
}


int skel::Engine::Initialize()
{
    Log::Init();
    m_console = std::make_shared<ConsolePanel>();

    m_windowWidth = m_game->GetStartupSettings().startupWindowWidth;
    m_windowHeight = m_game->GetStartupSettings().startupWindowHeight;


    SKEL_CORE_INFO("--------------------------------------");
    SKEL_CORE_INFO("|          SKEL FRAMEWORK v1.0       |");
    SKEL_CORE_INFO("--------------------------------------");


    std::string buildType;
#ifdef _DEBUG
    buildType = "Build: Debug   | ";
#else
    buildType = "Build: Release | ";
#endif

    const std::string buildDate = __DATE__;
    const std::string buildTime = __TIME__;
    buildType += buildDate + " " + buildTime;

    SKEL_CORE_INFO(buildType);



    if (InitializeGLFW())
    {
        SKEL_CORE_CRITICAL("Couldn't initialize skel Engine, shutting down.");
        ShutdownGLFW();
        return -1;
    }

   

    const char* rawVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* rawRenderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    std::string versionStr = rawVersion;
    std::string rendererStr = rawRenderer;

    std::string openglVersion;
    if (!versionStr.empty()) {
        size_t firstSpace = versionStr.find(' ');
        if (firstSpace != std::string::npos)
            openglVersion = "OpenGL " + versionStr.substr(0, firstSpace);
        else
            openglVersion = "OpenGL " + versionStr;
    }

    size_t slashPos = rendererStr.find('/');
    if (slashPos != std::string::npos)
        rendererStr = rendererStr.substr(0, slashPos);

    // Log
    SKEL_CORE_INFO("[Renderer] " + openglVersion + " | " + rendererStr);

    int width = 0, height = 0;
    glfwGetWindowSize(m_window, &width, &height);

    const GLFWmonitor* monitor = glfwGetWindowMonitor(m_window);
    const bool isFullscreen = monitor != nullptr;

    const std::string mode = isFullscreen ? "Fullscreen" : "Windowed";

    SKEL_CORE_INFO("[Window] " + std::to_string(width) + " x " + std::to_string(height) + " | " + mode);

    m_consoleToggleKey = m_game->GetStartupSettings().consoleToggleKey;

    m_uiManager.Initialize(m_window);
    m_renderer = new Renderer(m_game->GetStartupSettings().rendererWidth, m_game->GetStartupSettings().rendererHeight);
    m_viewportPanel = std::make_shared<ViewportPanel>();
    m_viewportPanel->Initialize(m_renderer);

    m_statsPanel = std::make_shared<StatPanel>(600);

    m_uiManager.RegisterPanel(m_console);
    m_uiManager.RegisterPanel(m_statsPanel);
    m_uiManager.RegisterPanel(m_viewportPanel);
    m_statsPanel->SetEnabled(m_game->GetStartupSettings().showStatsPanelOnStartup);


    m_console->SetEnabled(m_game->GetStartupSettings().showConsolePanelOnStartup);


    SKEL_CORE_INFO("[RenderTarget] " + std::to_string(m_renderer->GetWidth()) + " x " + std::to_string(m_renderer->GetHeight()));

    RegisterDefaultCommands();

    m_game->Initialize();
    SKEL_CORE_INFO("[GAME] Initialized");
    SKEL_CORE_INFO("--------------------------------");



    return 1;
}

// ReSharper disable once CppNotAllPathsReturnValue
int skel::Engine::InitializeGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_game->GetStartupSettings().windowTitle.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        SKEL_CORE_CRITICAL("Failed to create GLFW m_window");
        glfwTerminate();
        return -1;
    }
    glfwSetWindowUserPointer(m_window, this);

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SKEL_CORE_CRITICAL("Failed to initialize GLAD");
        return -1;
    }

    glViewport(0, 0, m_windowWidth, m_windowHeight);

    EnableVSync(m_game->GetStartupSettings().vsyncEnabled);

    // setting call backs
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_window, cursor_position_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetKeyCallback(m_window, key_callback);



    return 0;
}

void skel::Engine::RegisterDefaultCommands()
{

    m_console->RegisterCommand("quit", [this](const std::vector<std::string>& args) {
        CloseApplication();
    	return true;
        },
        "quit",
        "Closes the application.",
        "Closes the application."
    );

    m_console->RegisterCommand("stats", [this](const std::vector<std::string>& args) {
        if (!args.empty())
        {
	        if (args.at(0) == "reset")
	        {
                ImGui::ClearWindowSettings("Stats");
                m_statsPanel->SetEnabled(true);
                return true;
	        }
            else
            {
                return false;
            }
        }

        m_statsPanel->SetEnabled(!m_statsPanel->IsEnabled());
        return true;
        },
        "stats [?reset]",
        "Toggle the \"Stats\" ImGui panel on/off. ",
        "Toggles the real‑time stats panel (FPS, ms, 10 % / 1 % lows).\n [reset] to reset position of the stats window"
    );


    m_console->RegisterCommand("windowres", [this](const std::vector<std::string>& args) {
        m_console->PushLog("Window: " + std::to_string(m_windowWidth) + " x " + std::to_string(m_windowHeight));
        return true;
        },
        "windowres",
        "Print the current window resolution.",
        "Prints current window size in pixels.\nOutput, e.g.: Window: 1920 x 1080."
    );

    m_console->RegisterCommand("renderres", [this](const std::vector<std::string>& args) {
        m_console->PushLog("Render Target: " + std::to_string(m_renderer->GetWidth()) + " x " + std::to_string(m_renderer->GetHeight()));
        return true;
        },
        "renderres",
        "Print the fixed render‑target resolution.",
        "Prints the internal render‑target resolution.\nRender Target: 640 x 360"
    );


    m_console->RegisterCommand("viewportres", [this](const std::vector<std::string>& args) {

        const int2& sizeTotal = m_viewportPanel->GetTotalViewportSize();
        const int2& size = m_viewportPanel->GetUsedViewportSize();

        m_console->PushLog("Viewport: " + std::to_string(sizeTotal.x) + " x " + std::to_string(sizeTotal.y) + " (" +std::to_string(size.x) + " x " + std::to_string(size.y) + ")");
        return true;
        },
        "viewportres",
        "Print the viewport resolution and the used viewport resolution.",
        "Prints the total viewports resolution size and\nthe used resolution so it keeps the aspect ratio with the renderer.\nViewport: 640 x 360 (640 x 360)."
    );


    m_console->RegisterCommand("vsync", [this](const std::vector<std::string>& args) {

        if (args.empty())
        {
            m_console->PushLog(std::string("VSync: ") + (m_VSyncEnabled ? "On" : "Off"));
            return true;
        }
    
        if (args.at(0) == "on")
        {
            EnableVSync(true);
        }
        else if (args.at(0) == "off")
        {
            EnableVSync(false);
        }
        else
        {
            return false;
        }

    	m_console->PushLog(std::string("VSync: ") + (m_VSyncEnabled ? "On" : "Off"));
    	return true;

        },
        "vsync [?on/off]",
        "Enables/disables vsync or shows current state",
        "If called without arguments, shows whether VSync is enabled.\nIf passed true or false, enables or disables VSync accordingly."
    );


 

}


// ReSharper disable once CppMemberFunctionMayBeStatic
void skel::Engine::ShutdownGLFW()
{
    glfwTerminate();
}


void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
	if (skel::Engine* engine = static_cast<skel::Engine*>(glfwGetWindowUserPointer(window)))
        engine->OnFramebufferResize(width, height);
}

void cursor_position_callback(GLFWwindow* window, const double xpos, const double ypos)
{
	skel::Engine::GetInstance().GetInput().UpdateMousePosition(skel::int2(static_cast<int>(xpos), static_cast<int>(ypos)));
}

void mouse_button_callback(GLFWwindow* window, const int button, const int action, int mods)
{
    if (action == GLFW_PRESS) skel::Engine::GetInstance().GetInput().HandleMouseDown(button);
    else if (action == GLFW_RELEASE) skel::Engine::GetInstance().GetInput().HandleMouseUp(button);
    
}

void scroll_callback(GLFWwindow* window, double xoffset, const double yoffset)
{
    skel::Engine::GetInstance().GetInput().UpdateScrollDelta(static_cast<float>(yoffset));
}

void key_callback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
{
    if (action == GLFW_PRESS) skel::Engine::GetInstance().GetInput().HandleKeyDown(key);
    else if (action == GLFW_RELEASE) skel::Engine::GetInstance().GetInput().HandleKeyUp(key);
}
