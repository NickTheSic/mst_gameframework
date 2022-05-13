#pragma once
#include "mstgl.h"
#include "mstglm.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include <string>

namespace mst
{
    struct VertexData
    {
        v2f pos;
        Color color;
    };

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

    class QuadRenderer
    {
    public:
        QuadRenderer(unsigned int BatchCount);
        ~QuadRenderer();
        void Init(unsigned int BatchCount);

        // adds a rect where pos = lower left and size = top right
        void AddRect(const v2f& pos, const v2f& size, const Color& c = Color(255, 255, 255));
        void AddCenteredQuad(const v2f& pos, const v2f& size, const Color& c = Color(255,255,255));

        void StartRender();
        void EndRender();

        RendererData rd;
    };

    struct GlyphVertexData
    {
        v2f pos;
        Color color;
    };

    class TextRenderer
    {
    public:
        TextRenderer(unsigned int BatchCount, const std::string& FileName);
        void Init(unsigned int BatchCount, const std::string& FileName);

        void StartRender();
        void EndRender();

        RendererData rd; 
        FT_Library ft;
        FT_Face face;
    };

    void InitShaderCode(unsigned int& program, const char* vertexShaderSource, const char* fragmentShaderSource);
    void InitColourShader(unsigned int& Program);
    void CompileShader(unsigned int& shader, unsigned int type, const char* shaderSource);
}
