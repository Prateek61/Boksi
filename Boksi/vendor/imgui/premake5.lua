project "ImGui"
    kind "StaticLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "imgui/*.h",
        "imgui/*.cpp",
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        -- staticruntime "On"

    filter "system:linux"
        systemversion "latest"
        cppdialect "C++17"
        -- staticruntime "On"
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"