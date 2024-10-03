#pragma once

#include <Boksi/Core/Base.h>

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Boksi
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

// Core log macros
#define BK_CORE_FATAL(...)   ::Boksi::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define BK_CORE_ERROR(...)   ::Boksi::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BK_CORE_WARN(...)    ::Boksi::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BK_CORE_INFO(...)    ::Boksi::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BK_CORE_TRACE(...)   ::Boksi::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client log macros
#define BK_FATAL(...)        ::Boksi::Log::GetClientLogger()->critical(__VA_ARGS__)
#define BK_ERROR(...)        ::Boksi::Log::GetClientLogger()->error(__VA_ARGS__)
#define BK_WARN(...)         ::Boksi::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BK_INFO(...)         ::Boksi::Log::GetClientLogger()->info(__VA_ARGS__)
#define BK_TRACE(...)        ::Boksi::Log::GetClientLogger()->trace(__VA_ARGS__)
