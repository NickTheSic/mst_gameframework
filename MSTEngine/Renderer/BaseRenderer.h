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

	protected:
		BaseRenderer() = default;

		void FreeGraphicsMemory();		
		void InitBaseBufferObjects(unsigned int BatchCount, size_t VertexDataSize);

		void CompileShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
		void CompileShaderCode(unsigned int& shader, unsigned int type, const char* shaderSource);

		void SetUniform(const char* name, const v2f& val);
		int GetUniformLocation(const char* name);

		std::map<std::string, int> UniformLocations;

		unsigned int vbo;
		unsigned int vao;
		unsigned int ebo;
		unsigned int elementDrawCount;
		unsigned int maxVertices;
		unsigned int shaderProgram;
		unsigned int vertexCount;
	};
}