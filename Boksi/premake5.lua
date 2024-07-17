project "Boksi"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin/int/" .. outputdir .. "/%{prj.name}")

    pchheader "bkpch.h"
    pchsource "src/bkpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.spdlog}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }
     
    filter "system:windows"
        systemversion "latest"

        -- If build system is Visual Studio
        if _ACTION and string.startswith(_ACTION, "vs") then
            links
            {
                "%{Library.WinSock}",
                "%{Library.WinMM}",
                "%{Library.WinVersion}",
                "%{Library.BCrypt}"
            }
        else
            links
            {
                "%{Library.User32}",
                "%{Library.GDI32}"
            }
        end

    filter "configurations:Debug"
        defines "BK_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "BK_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "BK_DIST"
        runtime "Release"
        optimize "on"

        