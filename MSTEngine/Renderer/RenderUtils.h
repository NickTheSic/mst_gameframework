#pragma once

#include "mstgl.h"

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

    void FreeRenderData(RendererData& renderData);

    void InitColourShader(unsigned int& Program);
    void InitTextShader(unsigned int& Program);
    void InitShaderCode(unsigned int& program, const char* vertexShaderSource, const char* fragmentShaderSource);
    void CompileShader(unsigned int& shader, unsigned int type, const char* shaderSource);

    template <class ThisVertexData>
    void InitializeRendererData(RendererData& renderData, unsigned int BatchCount)
    {
        renderData.maxVertices = BatchCount * 4;
        unsigned int IndiceCount = BatchCount * 6;

        glGenVertexArrays(1, &renderData.vao);
        glGenBuffers(1, &renderData.vbo);
        glGenBuffers(1, &renderData.ebo);

        glBindVertexArray(renderData.vao);

        glBindBuffer(GL_ARRAY_BUFFER, renderData.vbo);
        glBufferData(GL_ARRAY_BUFFER,
            size_t(4) * size_t(BatchCount) * sizeof(ThisVertexData),
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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderData.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            size_t(IndiceCount) * sizeof(unsigned int),
            indices, GL_STATIC_DRAW);

        delete[] indices;
    }
}
