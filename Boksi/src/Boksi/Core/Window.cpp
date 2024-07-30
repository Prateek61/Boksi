#include "bkpch.h"
#include "Boksi/Core/Window.h"

#ifdef BK_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Boksi
{
	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
		#ifdef BK_PLATFORM_WINDOWS
			return CreateScope<WindowsWindow>(props);
		#else
			BK_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
		#endif
	}
}
