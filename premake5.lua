workspace "MST Framework"
    architecture "x86_64"
    startproject "Sandbox"

    debugdir "%{wks.location}"

    configurations
    {
        "Debug",
        "Release"
    }

    platforms
    {
        "Win64"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
    targetdir ("%{wks.location}/Build/".. outputdir.."/libs/")
    objdir ("%{wks.location}/Build/bin-obj/" .. outputdir)

filter "configurations:Debug"
    defines{"DEBUG", "SLOW_CODE"}
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    defines{"RELEASE", "NDEBUG"}
    runtime "Release"
    optimize "on"

filter {}

include "MSTEngine/MSTEngine.lua"
include "Sandbox"

group "Config"
include "ProjectConfig.lua"
group "ThirdParty"
include "ThirdParty/ThirdParty.lua"
group ""
