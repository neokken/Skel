#pragma once

#include <spdlog/sinks/callback_sink.h>

#include "UI/UIPanel.h"

namespace skel
{

	class ConsolePanel : public UIPanel
	{
		std::shared_ptr<spdlog::sinks::callback_sink_mt> m_callbackSink;

	public:
		using CommandFn = std::function<bool(const std::vector<std::string>&)>;

		ConsolePanel();
		~ConsolePanel() override;


		// Register a custom command, e.g. RegisterCommand("quit", [](args){ ... });
		void RegisterCommand(const std::string& name, CommandFn fn, const std::string& arguments, const std::string& shortDescription, const std::string& longDescription);


		void Render() override;
		
		


		void PushLog(std::string line, const ImVec4& color= { 1.f,1.f,1.f,1.f });

		void ExecCommand(const std::string& line);

		void AutoScrollToBottom() { m_scrollToBottom = true; }
	private:
		struct LogLine
		{
			std::string text;
			ImVec4 color{1.f,1.f,1.f,1.f};
		};

		std::deque<LogLine> m_Buffer;                      
		ImGuiTextFilter m_filter;                      
		bool m_scrollToBottom = true;      

		std::vector<std::string> m_history; // Command history
		int m_historyPos = -1;              // -1 = current input


		struct CommandData
		{
			std::string arguments;
			std::string shortDescription;
			std::string longDescription;
			CommandFn callback;
		};


		std::vector<std::string>                     m_commandOrder;
		std::unordered_map<std::string, CommandData> m_commands;


	};

}
