project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin/int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Boksi/src",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{wks.location}/Boksi/vendor"
    }

    links
    {
        "Boksi"
    }

    filter "system:windows"
        systemversion "latest"

        -- If build system is Visual Studio
        if _ACTION and string.startswith(_ACTION, "vs") then
        else
            links
            {
                "GLFW",
                "GLAD",
                "ImGui",
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