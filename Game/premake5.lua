project "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs {
        "src",
        "include",
        "../SkelFramework/include", 
        "../external/GLFW/include",
        "../external/glad/include",
        "../external/spdlog/include", 
        "../external/imgui", 
    }

    links {
        "SkelFramework",          
        "GLFW",
        "glad",
        "opengl32.lib",
        "imgui"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"