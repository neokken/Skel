workspace "Skel"
    architecture "x86_64"
    startproject "Game"

    configurations { 
        "Debug", 
        "Release" 
    }

    filter "system:windows"
        buildoptions { "/utf-8" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "external/GLFW"
    include "external/glad"
    include "external/imgui"
group ""

include "SkelFramework" 
include "Game" 