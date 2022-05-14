project "freetype"
	kind "staticlib"
	language "C"

	defines
	{
		"_LIB",
		"FT2_BUILD_LIBRARY",
	}

	includedirs
	{
		"%{wks.location}/ThirdParty/freetype/include"
	}

	files
	{
		"**c", "**.h", "**.cpp", "**.hpp", "*.lua"
	}