project "DiceGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/Build/" ..outputdir)
	debugdir ("%{wks.location}/")

	includedirs
	{
		"%{wks.location}/ThirdParty",
		"%{wks.location}/ThirdParty/freetype/include",
		"%{wks.location}/MSTEngine",
"C:/Program Files (x86)/OpenAL 1.1 SDK/include",
	}

	dependson
	{
		"MSTEngine",
	}

	files
	{
		"**c", "**.h", "**.cpp", "**.hpp", "*.lua"
	}
    
	libdirs
	{
		("%{wks.location}/Build/".. outputdir.."/libs/"),
"C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win64",
	}

	links
	{
		"MSTEngine",
	}

	filter "Release"
        kind "WindowedApp"
        entrypoint ("mainCRTStartup")

	filter{}