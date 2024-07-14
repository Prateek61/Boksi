#pragma once

#include "Boksi/Core/PlatformDetection.h"
#include <memory>

#ifdef BK_DEBUG
#if defined(BK_PLATFORM_WINDOWS)
#define BK_DEBUGBREAK() __debugbreak()
#elif defined(BK_PLATFORM_LINUX)
#include <signal.h>
#define BK_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#else
#define BK_DEBUGBREAK()
#endif

#define BK_EXPAND_MACRO(x) x
#define BK_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define BK_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Boksi
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Boksi/Core/Log.h"
#include "Boksi/Core/Assert.h"