#include "Shader.h"

#include "mstgl.h"
#include "mstDebug.h"
#include <string>

#if defined PLATFORM_WEB || defined __EMSCRIPTEN__
#define SHADER_VERSION_HEADER "#version 300 es \n precision mediump float; \n"
#else
#define SHADER_VERSION_HEADER  "#version 330 core\n"
#endif

namespace Shader
{
	struct Shaders
	{
		unsigned int Programs[(unsigned char)Type::SHADER_TYPE_COUNT] = {};
		
		~Shaders()
		{
			for (unsigned char i = 0; i < (unsigned char)Type::SHADER_TYPE_COUNT; i++)
			{
				if (Programs[i] != 0)
				{
					glDeleteProgram(Programs[i]);
				}
			}
		}
	};

	Shaders shaders;

	unsigned int CompileShaderCode(unsigned int type, const char* shaderSource)
	{	
		unsigned int shader = glCreateShader(type);

		glShaderSource(shader, 1, &shaderSource, 0);
		glCompileShader(shader);

		int  success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "Error compiling shader: " << type << std::endl;
			std::cout << "\t" << infoLog << std::endl;
		}

		return shader;
	}

	unsigned int CompileShader(const char* vertexCode, const char* fragmentCode)
	{
		unsigned int vertexShader, fragmentShader;
		vertexShader = CompileShaderCode(GL_VERTEX_SHADER, vertexCode);
		fragmentShader = CompileShaderCode(GL_FRAGMENT_SHADER, fragmentCode);

		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// check program error
		{
			int  success;
			char infoLog[512];
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
				std::cout << "Error linking Shader Program" << std::endl;
				std::cout << infoLog << std::endl;
			}
		}

		glUseProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}

	unsigned int CompileShaderProgram(Type t)
	{
		std::string vertex(SHADER_VERSION_HEADER);
		std::string fragment(SHADER_VERSION_HEADER);

		switch (t)
		{
			case Type::QUAD:
			{
				vertex.append(
					"layout (location = 0) in vec2 aPos;\n"
					"layout (location = 1) in vec3 aColour;\n"
					"uniform vec2 u_WorldSize;\n"
					"uniform vec2 u_CameraPos;\n"
					"uniform float u_CameraZoom;\n"
					"out vec3 oColour;\n"
					"void main()\n"
					"{\n"
					"oColour = aColour;\n"
					"vec2 pos = aPos;\n"
					"pos -= u_CameraPos;\n"
					"pos /= u_WorldSize * 0.5;\n"
					"gl_Position = vec4(pos, 0, 1.0);\n"
					"}\0"
				);

				fragment.append(
					"out vec4 FragColor;\n"
					"in vec3 oColour;\n"
					"void main(){\n"
					"FragColor = vec4(oColour, 1.0);\n"
					"}\0"
				);
			} break;

			case Type::FONT:
			{
				vertex.append(
					"layout (location = 0) in vec2 aPos;    \n"
					"layout (location = 1) in vec3 aColour; \n"
					"layout (location = 2) in vec2 size;    \n"
					"layout (location = 3) in vec2 InCoords;\n"
					"out vec2 TexCoords;\n"
					"out vec3 oColour;\n"
					"uniform vec2 u_WorldSize;\n"
					"uniform vec2 u_CameraPos;\n"
					"void main()\n"
					"{\n"
					"vec2 pos = aPos;\n"
					"pos -= u_CameraPos;\n"
					"pos /= u_WorldSize * 0.5;\n"
					"gl_Position = vec4(pos, 0, 1.0);\n"
					"TexCoords = InCoords;\n"
					"oColour = aColour;\n"
					"}\0"
				);

				fragment.append(
					"out vec4 FragColor;\n"
					"in vec3 oColour;\n"
					"in vec2 TexCoords;\n"
					"uniform sampler2D text;\n"
					"void main(){\n"
#if defined PLATFORM_WEB || defined __EMSCRIPTEN__
					"FragColor = vec4(oColour, texture(text, TexCoords).a);\n"
#else
					"FragColor = vec4(oColour, texture(text, TexCoords).r);\n"
#endif
					"}\0"
				);
			} break;

			case Type::SPRITESHEET:
			{
			} break;

			case Type::MY_UI:
			{
			} break;

			default:
			{
			} break;
		}

		return CompileShader(vertex.c_str(), fragment.c_str());
	}

	unsigned int GetShader(Type t)
	{
		if (shaders.Programs[(unsigned char)t] == 0)
		{
			shaders.Programs[(unsigned char)t] = CompileShaderProgram(t);
		}

		return shaders.Programs[(unsigned char)t];
	}
}