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
    include "Boksi/vendor/GLAD",
    include "Boksi/vendor/GLFW_premake",
group ""

gorup "Core"
    include "Boksi"
group ""

group "Sandbox"
    include "Sandbox"
group ""