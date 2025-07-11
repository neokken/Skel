#pragma once
#include "Renderer/Renderer.h"
#include "UI/UIPanel.h"

namespace skel
{
	

class ViewportPanel : public UIPanel
{
public:
	ViewportPanel() = default;
	void Initialize(Renderer* renderer);

	virtual void Render() override;

	bool IsFocused() const { return m_focused; }
	bool IsHovered() const { return m_hovered; }

	const int2& GetUsedViewportSize() const { return m_viewportSize; }
	const int2& GetTotalViewportSize() const { return m_viewportTotalSize; }

	int2 PanelToRenderTargetCoords(const int2& panelPos);
private:
	Renderer* m_renderer{nullptr};

	int2 m_viewportSize{ 0, 0 };
	int2 m_viewportTotalSize{ 0, 0 };

	int2 m_offset{ 0 };


	bool m_focused{false};
	bool m_hovered{false};

};


}
