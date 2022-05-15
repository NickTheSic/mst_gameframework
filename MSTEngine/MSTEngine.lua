project "MSTEngine"
	kind "StaticLib"
	language "C++"

	includedirs
	{
		"%{wks.location}/MSTEngine",
		"%{wks.location}/ThirdParty",
		"%{wks.location}/ThirdParty/freetype/include",
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
		("%{wks.location}/Build/".. outputdir.."/libs/")
	}

	links
	{
		"gdi32",
		"opengl32",
		"stb_image",
		"freetype",
	}
