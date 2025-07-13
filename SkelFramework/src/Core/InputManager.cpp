#include "skelpch.h"
#include "Core/InputManager.h"

#include "imgui_internal.h"
#include "Core/Engine.h"

struct ImGuiWindow;

void skel::InputManager::UpdateMousePosition(const int2& mousePos)
{
	const int2 rawPosition = mousePos;
	int2 viewPosition = rawPosition;

	ViewportPanel& viewport = Engine::GetInstance().GetViewport();

	int window_x, window_y;
	glfwGetWindowPos(Engine::GetInstance().GetGLFWWindow(), &window_x, &window_y);

	if (const ImGuiWindow* window = ImGui::FindWindowByName("Viewport"))
	{
		viewPosition -= int2(static_cast<int>(window->Pos.x - window_x), static_cast<int>(window->Pos.y - window_y));
		viewPosition.x = std::clamp(viewPosition.x, 0, static_cast<int>(window->Size.x));
		viewPosition.y = std::clamp(viewPosition.y, 0, static_cast<int>(window->Size.y));
	}

	const int2 localPosition = viewport.PanelToRenderTargetCoords(viewPosition);



	m_mouseRawDelta = m_mouseRawPosition - rawPosition;
	m_mouseRawPosition = rawPosition;

	m_mouseViewportDelta = m_mouseViewportPosition - viewPosition;
	m_mouseViewportPosition = viewPosition;

	m_mouseDelta = m_mousePosition - localPosition;
	m_mousePosition = localPosition;

}

void skel::InputManager::HandleKeyDown(const int key)
{
	m_keys[key] = KeyState::Just_Down;
}

void skel::InputManager::HandleKeyUp(const int key)
{
	m_keys[key] = KeyState::Just_Up;
}

void skel::InputManager::HandleMouseDown(const int button)
{
	m_mouseButtons[button] = KeyState::Just_Down;
}

void skel::InputManager::HandleMouseUp(const int button)
{
	m_mouseButtons[button] = KeyState::Just_Up;
}

void skel::InputManager::Update(float deltaTime)
{
	for (auto& key : m_keys)
	{
		if (key == KeyState::Just_Down)
		{
			key = KeyState::Down;
		}
		else if (key == KeyState::Just_Up)
		{
			key = KeyState::Up;
		}
	}

	for (auto& button : m_mouseButtons)
	{
		if (button == KeyState::Just_Down)
		{
			button = KeyState::Down;
		}
		else if (button == KeyState::Just_Up)
		{
			button = KeyState::Up;
		}
	}

	m_mouseDelta = int2(0);
	m_scrollDelta = 0.f;
}

bool skel::InputManager::IsKeyDown(const int key, const bool ignoreBlocked) const
{
	return (ignoreBlocked || Engine::GetInstance().GetViewport().IsFocused()) && (m_keys[key] == KeyState::Just_Down || m_keys[key] == KeyState::Down);
}

bool skel::InputManager::IsKeyUp(const int key, const bool ignoreBlocked) const
{
	return (ignoreBlocked || Engine::GetInstance().GetViewport().IsFocused()) && (m_keys[key] == KeyState::Just_Up || m_keys[key] == KeyState::Up);
}

bool skel::InputManager::IsKeyJustDown(const int key, const bool ignoreBlocked) const
{
	return (ignoreBlocked || Engine::GetInstance().GetViewport().IsFocused()) && m_keys[key] == KeyState::Just_Down;
}

bool skel::InputManager::IsKeyJustUp(const int key, const bool ignoreBlocked) const
{
	return (ignoreBlocked || Engine::GetInstance().GetViewport().IsFocused()) && m_keys[key] == KeyState::Just_Up;
}

bool skel::InputManager::IsMouseDown(const int button, const bool ignoreBlocked) const
{
	return (ignoreBlocked || Engine::GetInstance().GetViewport().IsFocused()) && (m_mouseButtons[button] == KeyState::Just_Down || m_mouseButtons[button] == KeyState::Down);
}

bool skel::InputManager::IsMouseUp(const int button, const bool ignoreBlocked) const
{
	return (ignoreBlocked || Engine::GetInstance().GetViewport().IsFocused()) && (m_mouseButtons[button] == KeyState::Just_Up || m_mouseButtons[button] == KeyState::Up);
}

bool skel::InputManager::IsMouseJustDown(const int button, const bool ignoreBlocked) const
{
	return (ignoreBlocked || Engine::GetInstance().GetViewport().IsFocused()) && m_mouseButtons[button] == KeyState::Just_Down;
}

bool skel::InputManager::IsMouseJustUp(const int button, const bool ignoreBlocked) const
{
	return (ignoreBlocked || Engine::GetInstance().GetViewport().IsFocused()) && m_mouseButtons[button] == KeyState::Just_Up;
}
