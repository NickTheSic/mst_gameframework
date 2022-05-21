#pragma once

#include "mstglm.h"
#include <map>
#include <string>

namespace mst
{ 
	class BaseRenderer
	{
	public:
		virtual ~BaseRenderer() = default;

		void UseProgram();
		void SetUniform(const char* name, const v2f& val);
	protected:
		BaseRenderer() = default;

		void FreeGraphicsMemory();		
		void InitBaseBufferObjects(unsigned int BatchCount, size_t VertexDataSize);

		void CompileShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
		void CompileShaderCode(unsigned int& shader, unsigned int type, const char* shaderSource);

		int GetUniformLocation(const char* name);

		std::map<std::string, int> UniformLocations;

		unsigned int vbo              = 0;
		unsigned int vao              = 0;
		unsigned int ebo              = 0;
		unsigned int elementDrawCount = 0;
		unsigned int maxVertices      = 0;
		unsigned int shaderProgram    = 0;
		unsigned int vertexCount      = 0;
	};
}