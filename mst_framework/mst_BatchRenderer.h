#pragma once
#include "mst_gl_header.h"
#include "mst_vec2.h"
#include "mst_vec3.h"

namespace mst
{
    struct VertexData
    {
        v2f pos;
        Color color;
    };

    class QuadRenderer
    {
    public:
        QuadRenderer(unsigned int BatchCount);
        void Init(unsigned int BatchCount);

        // adds a rect where pos = lower left and size = top right
        void AddRect(const v2f& pos, const v2f& size);
        void AddCenteredQuad(const v2f& pos, const v2f& size);

        void StartRender();
        void EndRender();

        unsigned int vbo;
        unsigned int vao;
        unsigned int ebo;
        unsigned int elementDrawCount;
        unsigned int maxVertices;
        unsigned int shaderProgram;
        unsigned int vertexCount;

        unsigned int DrawsPerFrame = 0;
    };

    void InitShaderCode(unsigned int& program,
            const char* vertexShaderSource,
            const char* fragmentShaderSource);
    void InitColourShader(unsigned int& Program);
    void CompileShader(unsigned int& shader, unsigned int type, const char* shaderSource);
}
