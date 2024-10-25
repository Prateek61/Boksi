project "Flecs"
    kind "StaticLib"
    language "C"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "flecs.c",
        "flecs.h"
    }

    includedirs
    {
        "."
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        defines "FLECS_DBG"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        defines "FLECS_NDEBUG"
    
    filter "configurations:Dist"
        runtime "Release"
        optimize "on"
        symbols "off"
        defines "FLECS_NDEBUG"