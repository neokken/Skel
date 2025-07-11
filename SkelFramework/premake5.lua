project "SkelFramework"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/"     .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- Precompiled header lives in SkelFramework/src
    pchheader "skelpch.h"
    pchsource "src/skelpch.cpp"

    files {
        "include/**.h",
        "src/**.cpp",
    }

    includedirs {
        "include",
        "../external/glad/include",
        "../external/glfw/include",
        "../external/imgui",
        "../external/spdlog/include",
        "../external/stbimage/include",
    }

    links { "glad", "GLFW", "imgui", "opengl32.lib" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"