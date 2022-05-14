project "MSTEngine"
	kind "StaticLib"
	language "C++"

	includedirs
	{
		"%{wks.location}/MSTEngine",
		"%{wks.location}/ThirdParty",
	}

	dependson
	{
		"stb_image",
		"stb_truetype",
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
		"stb_truetype",
	}
