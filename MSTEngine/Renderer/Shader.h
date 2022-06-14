#pragma once

namespace Shader
{
	enum class Type : unsigned char
	{
		QUAD,
		FONT,
		SPRITESHEET,
		MY_UI,

		SHADER_TYPE_COUNT
	};

	unsigned int GetShader(Type t);
}