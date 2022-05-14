project "Config"
	kind "None"

	files
	{
		"**.lua",
		"**.bat",
		".gitignore",
	}

	buildcommands 
	{
		"call vs2019.bat"
	}
