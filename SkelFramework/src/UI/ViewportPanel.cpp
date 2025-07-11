#include "skelpch.h"
#include "UI/ViewportPanel.h"

#include "Core/Engine.h"
#include "imgui.h"


void skel::ViewportPanel::Initialize(Renderer* renderer)
{
    m_renderer = renderer;
}

void skel::ViewportPanel::Render()
{
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse;


    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport", nullptr, window_flags);

    m_focused = ImGui::IsWindowFocused();
    m_hovered = ImGui::IsWindowHovered();
    bool blockInput = !(m_focused && m_hovered);

    // Disable game input when over the viewport
    // Application::Get().GetImGuiLayer()->BlockEvents(blockInput); <-- your call


    const ImVec2 panelSize = ImGui::GetContentRegionAvail();

    m_viewportTotalSize = { static_cast<int>(panelSize.x), static_cast<int>(panelSize.y) };

    const float targetAspect = static_cast<float>(m_renderer->GetWidth()) / static_cast<float>(m_renderer->GetHeight());
    const float panelAspect = panelSize.x / panelSize.y;


    ImVec2 imageSize;
    ImVec2 offset = ImVec2(0, 0);

    // Fit while maintaining aspect ratio (with black bars)
    if (panelAspect > targetAspect)
    {
        // Panel is too wide
        imageSize.y = panelSize.y;
        imageSize.x = panelSize.y * targetAspect;
        offset.x = (panelSize.x - imageSize.x) * 0.5f;
    }
    else
    {
        // Panel is too tall
        imageSize.x = panelSize.x;
        imageSize.y = panelSize.x / targetAspect;
        offset.y = (panelSize.y - imageSize.y) * 0.5f;
    }
    m_offset = { static_cast<int>(offset.x), static_cast<int>(offset.y) };

    m_viewportSize = { static_cast<int>(imageSize.x), static_cast<int>(imageSize.y) };

    // Center the image with black bars
    ImGui::SetCursorPos(offset);

    ImGui::Image(
        (void*)(intptr_t)m_renderer->GetOutputTexture(),
        imageSize,
        ImVec2{ 0, 1 }, ImVec2{ 1, 0 } // Flip vertically
    );

    ImGui::End();
    ImGui::PopStyleVar();
}

skel::int2 skel::ViewportPanel::PanelToRenderTargetCoords(const int2& panelPos)
{
    // Subtract the offset to get coords relative to the image
    int2 relativePos = panelPos - m_offset;

    // Clamp to image size
    relativePos.x = std::clamp(relativePos.x, 0, m_viewportSize.x);
    relativePos.y = std::clamp(relativePos.y, 0, m_viewportSize.y);

    // Scale up to actual render target size
    const float scaleX = static_cast<float>(m_renderer->GetWidth()) / static_cast<float>(m_viewportSize.x);
    const float scaleY = static_cast<float>(m_renderer->GetHeight()) / static_cast<float>(m_viewportSize.y);

    int2 rtPos;
    rtPos.x = static_cast<int>(static_cast<float>(relativePos.x) * scaleX);
    rtPos.y = static_cast<int>(static_cast<float>(relativePos.y) * scaleY);

    return rtPos;
}
