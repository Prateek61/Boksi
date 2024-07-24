// #pragma once

#ifndef BK_DIST
	#define BK_ENABLE_ASSERTS
#endif

#include "Boksi/Core/PlatformDetection.h"

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstdint>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <initializer_list>
#include <unordered_map>
#include <unordered_set>

#include "Boksi/Core/Base.h"
#include "Boksi/Core/Log.h"
#include "Boksi/Core/Assert.h"



#ifdef BK_PLATFORM_WINDOWS
#include <Windows.h>
#endif