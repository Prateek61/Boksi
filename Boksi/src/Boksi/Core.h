#pragma once

#ifdef BK_PLATFORM_WINDOWS
	#ifdef BK_BUILD_DLL
		#define BOKSI_API __declspec(dllexport)
	#else
		#define BOKSI_API __declspec(dllimport)
	#endif
#else
	#error Boksi only supports Windows!
#endif
