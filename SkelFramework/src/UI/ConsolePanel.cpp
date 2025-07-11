#include "skelpch.h"
#include "UI/ConsolePanel.h"

#include <ranges>
#include <spdlog/sinks/callback_sink.h>

skel::ConsolePanel::ConsolePanel()
{

    m_callbackSink = std::make_shared<spdlog::sinks::callback_sink_mt>(
        [this](const spdlog::details::log_msg& msg) {
            spdlog::memory_buf_t buf;
            // create a temporary formatter on the fly
            spdlog::pattern_formatter formatter("%^[%T] [%l] %n: %v%$");
            formatter.format(msg, buf);

            ImVec4 color{ 1,1,1,1 };
            switch (msg.level)
            {
            case spdlog::level::trace:    color = ImVec4(.6f, .6f, .6f, 1); break;
            case spdlog::level::debug:    color = ImVec4(.7f, .7f, .7f, 1); break;
            case spdlog::level::info:     color = ImVec4(.2f, 1.f, .2f, 1); break;
            case spdlog::level::warn:     color = ImVec4(1.f, .8f, .2f, 1); break;
            case spdlog::level::err:      color = ImVec4(1.f, .2f, .2f, 1); break;
            case spdlog::level::critical: color = ImVec4(1.f, 0.f, 0.f, 1); break;
            default: break;
            }

            this->PushLog(fmt::to_string(buf), color);
        });

    // Set formatter by passing ownership:
    m_callbackSink->set_formatter(std::make_unique<spdlog::pattern_formatter>("%^[%T] %n: %v%$"));

    skel::Log::GetCoreLogger()->sinks().push_back(m_callbackSink);
    skel::Log::GetClientLogger()->sinks().push_back(m_callbackSink);



    
    RegisterCommand("help", 
        [this](const std::vector<std::string>& args) {
            if (args.empty())
            {
                PushLog("Available commands:");

                int longestArgumentText = 0;
                for (const auto& val : m_commands | std::views::values)
                {
                    longestArgumentText = std::max(longestArgumentText, static_cast<int>(val.arguments.length()));
                }


                for (auto& p : m_commandOrder)
                {
                    const auto& command = m_commands.at(p);
                    std::stringstream ss;

                    ss << std::left << std::setw(longestArgumentText) << command.arguments << " - " << command.shortDescription;

                    PushLog("  " + ss.str());
                }
                return true;
            }
            else
            {
                auto it = m_commands.find(args.at(0));
                if (it != m_commands.end())
                {
                    PushLog(it->second.arguments);
                    PushLog(it->second.longDescription);
                }
                else
                    PushLog("Could not find command \"" + args.at(0) + "\"", { 1,0,0,1 });

                return true;
            }

        },
		"help [?command]",
        "Show help for every command or one specific command.", 
        "No arg → lists every command.\nex: \"help help\" : shows this."
    );


    RegisterCommand("clear", [this](auto&&) { m_Buffer.clear(); return true; },
		"clear",
        "Clears the console.",
        "Removes all text from the console."
    );

}

skel::ConsolePanel::~ConsolePanel()
{
    skel::Log::GetCoreLogger()->sinks().erase(std::ranges::remove(skel::Log::GetCoreLogger()->sinks(), m_callbackSink).begin());
    skel::Log::GetClientLogger()->sinks().erase(std::ranges::remove(skel::Log::GetClientLogger()->sinks(), m_callbackSink).begin());
}

void skel::ConsolePanel::RegisterCommand(const std::string& name, CommandFn fn, const std::string& arguments,
	const std::string& shortDescription, const std::string& longDescription)
{
    m_commandOrder.push_back(name);
    m_commands[name] = { arguments, shortDescription,longDescription,std::move(fn) };
}

void skel::ConsolePanel::Render()
{

    if (!ImGui::Begin("Console", &m_enabled))
    {
        ImGui::End(); return;
    }

    /* filter input */
    m_filter.Draw("Filter", 180);

    /* main scrolling region */
    ImGui::Separator();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false,
        ImGuiWindowFlags_HorizontalScrollbar);

    for (const auto& line : m_Buffer)
    {
        if (!m_filter.PassFilter(line.text.c_str())) continue;
        ImGui::PushStyleColor(ImGuiCol_Text, line.color);
        ImGui::TextUnformatted(line.text.c_str());
        ImGui::PopStyleColor();
    }

    if (m_scrollToBottom)
    {
        ImGui::SetScrollHereY(1.0f);
        m_scrollToBottom = false;
    }
    ImGui::EndChild();
    ImGui::Separator();

    /* command‑line input */
    static char input[256] = {};
    bool reclaim_focus = false;

    if (ImGui::InputText("Input", input, sizeof(input),
        ImGuiInputTextFlags_EnterReturnsTrue |
        ImGuiInputTextFlags_CallbackHistory,
        [](ImGuiInputTextCallbackData* data)->int
        {
            auto* self = static_cast<ConsolePanel*>(data->UserData);
            if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
            {
                if (data->EventKey == ImGuiKey_UpArrow)
                {
                    if (self->m_historyPos == -1)
                        self->m_historyPos = static_cast<int>(self->m_history.size());
                    if (self->m_historyPos > 0)
                        --self->m_historyPos;
                }
                else if (data->EventKey == ImGuiKey_DownArrow)
                {
                    if (self->m_historyPos != -1 &&
                        ++self->m_historyPos >= static_cast<int>(self->m_history.size()))
                        self->m_historyPos = -1;
                }
                if (self->m_historyPos >= 0)
                {
                    std::string hist = self->m_history[self->m_historyPos];
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, hist.c_str());
                }
            }
            return 0;
        }, this))
    {
        std::string line = input;
        if (!line.empty())
        {
            ExecCommand(line);
            input[0] = '\0';
        }
        reclaim_focus = true;
    }

    /* focus IME after submit so you can keep typing */
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1);

    ImGui::End();
}

void skel::ConsolePanel::PushLog(std::string line, const ImVec4& color)
{
    m_Buffer.emplace_back(std::move(line),color);
    if (m_Buffer.size() > 1000) m_Buffer.pop_front();          // simple cap
    m_scrollToBottom = true;
}

void skel::ConsolePanel::ExecCommand(const std::string& line)
{
    PushLog("> " + line, { 0.35f, 0.8f, 1.0f, 1.0f });
    m_history.push_back(line);
    m_historyPos = -1;

    /* tokenize */
    std::stringstream ss(line);
    std::string cmd; ss >> cmd;

    std::vector<std::string> args;
    std::string arg;
    while (ss >> arg) args.push_back(arg);

    auto it = m_commands.find(cmd);
    if (it != m_commands.end())
    {
        bool success = it->second.callback(args);

        if (!success)
        {
            PushLog("Invalid use case for " + cmd, { 1,0,0,1 });
            PushLog(it->second.arguments, {1,0,0,1});
        }
    }
    else
        PushLog("Unknown command: " + cmd, { 1,0,0,1 });
}
