include "Dependencies.lua"

workspace "Boksi"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Boksi/vendor/GLAD"
    include "Boksi/vendor/GLFW"
    include "Boksi/vendor/imgui"
group ""

group "Core"
    include "Boksi"
group ""

group "Sandbox"
    include "Sandbox"
group ""