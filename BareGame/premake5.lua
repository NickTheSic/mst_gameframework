project "TemplateProj"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/Build/" ..outputdir)
	debugdir ("%{wks.location}/")

	includedirs
	{
		"%{wks.location}/ThirdParty",
		"%{wks.location}/ThirdParty/freetype/include",
		"%{wks.location}/MSTEngine",
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
		("%{wks.location}/Build/".. outputdir.."/libs/")
	}

	links
	{
		"MSTEngine",
	}

	filter "Release"
        kind "WindowedApp"
        entrypoint ("mainCRTStartup")

	filter{}