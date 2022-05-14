#pragma once

namespace mst
{
    struct RendererData
    {
        unsigned int vbo;
        unsigned int vao;
        unsigned int ebo;
        unsigned int elementDrawCount;
        unsigned int maxVertices;
        unsigned int shaderProgram;
        unsigned int vertexCount;

        unsigned int DrawsPerFrame = 0;
    };

    void InitColourShader(unsigned int& Program);
    void InitTextShader(unsigned int& Program);
    void InitShaderCode(unsigned int& program, const char* vertexShaderSource, const char* fragmentShaderSource);
    void CompileShader(unsigned int& shader, unsigned int type, const char* shaderSource);
}
