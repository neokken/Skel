#pragma once
#include "UI/UIPanel.h"
#include <deque>

namespace skel
{


class StatPanel : public UIPanel
{
public:
	StatPanel(int history = 100); // we keep track of the last x amount of samples

	void Render() override;

private:
    void ComputeStats();

    std::deque<float> m_samples;
    int m_maxHistory;

    float m_FPS = 0.0f;
    float m_frameMs = 0.0f;
    float m_movingAvgMs = 0.0f;
    float m_pct10Ms = 0.0f;
    float m_pct1Ms = 0.0f;

    const float m_emaAlpha = 0.03f;   // 5 % weight of newest sample
};

}
