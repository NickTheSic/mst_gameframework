project "MSTEngine"
	kind "StaticLib"
	language "C++"

	includedirs
	{
		"%{wks.location}/MSTEngine",
		"%{wks.location}/ThirdParty",
		"%{wks.location}/ThirdParty/stb_image",
		"%{wks.location}/ThirdParty/freetype/include",
		"C:/Program Files (x86)/OpenAL 1.1 SDK/include",
	}

	dependson
	{
		"stb_image",
		"freetype",
	}

	files
	{
		"*.lua",
		"**.cpp",
		"**.h",
	}
    
	libdirs
	{
		("%{wks.location}/Build/".. outputdir.."/libs/"),
		"C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win64",
	}

	links
	{
		"gdi32",
		"opengl32",
		"stb_image",
		"freetype",
		"OpenAL32"
	}
