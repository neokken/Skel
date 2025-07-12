#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace skel
{

	enum class KeyState : std::uint8_t
	{
	Up,
	Just_Up,
	Just_Down,
	Down
	};

	class InputManager
	{
	public:
	InputManager() = default;


	void UpdateMousePosition(const int2& mousePos);

	void UpdateScrollDelta(const float scrollDelta) { m_scrollDelta = scrollDelta; }


	void HandleKeyDown(int key); 
	void HandleKeyUp(int key);

	void HandleMouseDown(int button);
	void HandleMouseUp(int button);


	void Update(float deltaTime);


	[[nodiscard]] bool IsKeyDown(int key, bool ignoreBlocked=false) const;
	[[nodiscard]] bool IsKeyUp(int key, bool ignoreBlocked = false) const;
	[[nodiscard]] bool IsKeyJustDown(int key, bool ignoreBlocked = false) const;
	[[nodiscard]] bool IsKeyJustUp(int key, bool ignoreBlocked = false) const;


	[[nodiscard]] bool IsMouseDown(const int button, bool ignoreBlocked = false) const;
	[[nodiscard]] bool IsMouseUp(const int button, bool ignoreBlocked = false) const;

	[[nodiscard]] bool IsMouseJustDown(const int button, bool ignoreBlocked = false) const;

	[[nodiscard]] bool IsMouseJustUp(const int button, bool ignoreBlocked = false) const;
	
	
	[[nodiscard]] const int2& GetMousePosition() const { return m_mousePosition; }					// Render target position
	[[nodiscard]] const int2& GetMousePositionRaw() const { return m_mouseRawPosition; }			// Window position
	[[nodiscard]] const int2& GetMousePositionViewport() const { return m_mouseViewportPosition; }	// Viewport position

	[[nodiscard]] const int2& GetMouseDelta() const { return m_mouseDelta; }						// Render target delta
	[[nodiscard]] const int2& GetMouseDeltaRaw() const { return m_mouseRawDelta; }					// Window delta
	[[nodiscard]] const int2& GetMouseDeltaViewport() const { return m_mouseViewportDelta; }		// Viewport delta

	[[nodiscard]] float GetScrollDelta() const { return m_scrollDelta; }

	private:
	int2 m_mouseRawPosition{ 0 };
	int2 m_mouseViewportPosition{ 0 };
	int2 m_mousePosition{ 0 };

	int2 m_mouseRawDelta{ 0 };
	int2 m_mouseViewportDelta{ 0 };
	int2 m_mouseDelta{ 0 };


	float m_scrollDelta{ 0.f };

	KeyState m_keys[348]{ KeyState::Up }; // 348 GLFW_KEY_LAST
	KeyState m_mouseButtons[7]{ KeyState::Up }; // 7 GLFW_MOUSE_BUTTON_LAST
	};
}
