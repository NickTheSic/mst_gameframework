#include "BaseRenderer.h"
#include "mstgl.h"
#include <iostream>

namespace mst
{   
	void BaseRenderer::FreeGraphicsMemory()
	{
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(shaderProgram);
	}

	void BaseRenderer::InitBaseBufferObjects(unsigned int BatchCount, size_t VertexDataSize)
	{
        maxVertices = BatchCount * 4;
        unsigned int IndiceCount = BatchCount * 6;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
            size_t(4) * size_t(BatchCount) * VertexDataSize,
            NULL, GL_DYNAMIC_DRAW);

        unsigned int* indices = new unsigned int[IndiceCount];
        unsigned int offset = 0;
        for (size_t i = 0; i < IndiceCount; i += 6)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            size_t(IndiceCount) * sizeof(unsigned int),
            indices, GL_STATIC_DRAW);

        delete[] indices;
	}

	void BaseRenderer::CompileShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
        unsigned int vertex, fragment;

        CompileShaderCode(vertex, GL_VERTEX_SHADER, vertexShaderSource);
        CompileShaderCode(fragment, GL_FRAGMENT_SHADER, fragmentShaderSource);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertex);
        glAttachShader(shaderProgram, fragment);
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

        glDeleteShader(vertex);
        glDeleteShader(fragment);
	}

	void BaseRenderer::CompileShaderCode(unsigned int& shader, unsigned int type, const char* shaderSource)
	{
        shader = glCreateShader(type);
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
	}
    
    void BaseRenderer::UseProgram()
    {
        glUseProgram(shaderProgram);
    }

    void BaseRenderer::SetUniform(const char* name, const v2f& val)
    {
        int Loc = GetUniformLocation(name);
        glUniform2fv(Loc, 1, &val[0]);
    }

    void BaseRenderer::SetUniform(const char* name, const float& val)
    {
        int Loc = GetUniformLocation(name);
        glUniform1f(Loc, val);
    }

    int BaseRenderer::GetUniformLocation(const std::string& name)
    {
        auto It = UniformLocations.find(name);

        if (It == UniformLocations.end())
        {
            UniformLocations[name] = glGetUniformLocation(shaderProgram, name.c_str());
        }

        return UniformLocations[name];
    }
}
