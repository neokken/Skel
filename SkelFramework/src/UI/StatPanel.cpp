#include "skelpch.h"
#include "UI/StatPanel.h"

#include "Core/Engine.h"

skel::StatPanel::StatPanel(const int history)
	: m_maxHistory(history)
{
}

void skel::StatPanel::Render()
{
	const float dt = Engine::GetInstance().GetNonCampledDeltaTime();

	m_samples.push_back(dt);

	if (m_samples.size() > m_maxHistory)
		m_samples.pop_front();

	if (m_movingAvgMs == 0.0f)
		m_movingAvgMs = dt * 1000.0f;
	else
		m_movingAvgMs = (1.0f - m_emaAlpha) * m_movingAvgMs + m_emaAlpha * (dt * 1000.0f);

	ComputeStats();


    if (!ImGui::Begin("Stats", &m_enabled)) { ImGui::End(); return; }

    ImGui::Text("FPS        : %.2f", m_FPS);
    ImGui::Text("Frame ms   : %.2f", m_frameMs);
    ImGui::Separator();
    ImGui::Text("Moving avg : %.2f ms (%.1f fps)", m_movingAvgMs, 1000.f / m_movingAvgMs);
    ImGui::Text("10%% low    : %.2f ms (%.1f fps)", m_pct10Ms, 1000.f/m_pct10Ms);
    ImGui::Text("1%% low     : %.2f ms (%.1f fps)", m_pct1Ms, 1000.f / m_pct1Ms);

    ImGui::End();

}

void skel::StatPanel::ComputeStats()
{
    if (m_samples.empty()) return;

    /* current frame */
    const float dt = m_samples.back();
    m_frameMs = dt * 1000.0f;
    m_FPS = 1.0f / dt;

    /* copy & sort to compute percentiles */
    std::vector<float> sorted = { m_samples.begin(), m_samples.end() };
    std::ranges::sort(sorted);        

    auto pct = [&](float p) -> float
        {
            const size_t idx = static_cast<size_t>((1.0f - p) * (sorted.size() - 1));
            return sorted[idx] * 1000.0f;
        };

    m_pct10Ms = pct(0.10f);     // 10 % low
    m_pct1Ms = pct(0.01f);     // 1 % low
}
