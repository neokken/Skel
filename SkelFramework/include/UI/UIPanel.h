#pragma once

namespace skel
{
	class UIPanel
	{
	public:
		virtual ~UIPanel() = default;

		virtual void Render() = 0;

		bool IsEnabled() const { return m_enabled; }
		void SetEnabled(const bool enabled) { m_enabled = enabled; }

		
	protected:
		bool m_enabled{ true };
	};
}
