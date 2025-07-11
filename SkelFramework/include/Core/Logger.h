#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace skel
{
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }


	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;


	};

}


#define SKEL_CORE_TRACE(...)		::skel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SKEL_CORE_INFO(...)			::skel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SKEL_CORE_ERROR(...)		::skel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SKEL_CORE_WARN(...)			::skel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SKEL_CORE_CRITICAL(...)		::skel::Log::GetCoreLogger()->critical(__VA_ARGS__)


#define SKEL_TRACE(...)			::skel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SKEL_INFO(...)			::skel::Log::GetClientLogger()->info(__VA_ARGS__)
#define SKEL_ERROR(...)			::skel::Log::GetClientLogger()->error(__VA_ARGS__)
#define SKEL_WARN(...)			::skel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SKEL_CRITICAL(...)		::skel::Log::GetClientLogger()->critical(__VA_ARGS__)