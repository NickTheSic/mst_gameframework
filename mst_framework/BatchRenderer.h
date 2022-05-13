#pragma once
#include "mstgl.h"
#include "mstglm.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include <string>
#include <vector>

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

    struct GlyphData
    {
        v2i size;
        v2i bearing;
        unsigned int advance;
        unsigned int TextureId;
    };

    struct GlyphVertexData
    {
        v2f pos;
        Color color;
        GlyphData glyph;
    };

    class TextRenderer
    {
    public:
        TextRenderer(unsigned int BatchCount, const std::string& FileName);
        ~TextRenderer();
        void Init(unsigned int BatchCount, const std::string& FileName);

        void InitFont(std::string FileName);

        void StartRender();
        void EndRender();

        void RenderString(const std::string& String);
        //void RenderChar(unsigned char c);

        RendererData rd;

        std::vector<GlyphData> Glyphs;
    };

    void InitColourShader(unsigned int& Program);
    void InitTextShader(unsigned int& Program);
    void InitShaderCode(unsigned int& program, const char* vertexShaderSource, const char* fragmentShaderSource);
    void CompileShader(unsigned int& shader, unsigned int type, const char* shaderSource);
}
