#pragma once

#include "mstglm.h"
#include "BaseRenderer.h"
#include <string>
#include <vector>

namespace mst
{
    struct GlyphData
    {
        v2i size;
        v2i bearing;
        float xoffset;        // works for ascii characters
        unsigned int advance; // works for ascii characters
    };

    struct GlyphVertexData
    {
        v2f pos;
        Color color;

        v2f size;
        v2f coords;
    };

    class TextRenderer : public BaseRenderer
    {
    public:
        TextRenderer() = default;
        TextRenderer(unsigned int BatchCount, const char* FilePath);
        ~TextRenderer();
        void Init(unsigned int BatchCount, const char* FilePath);
        void InitFontSheet(const char* FilePath);
        void InitShader();
        void StartRender();
        void EndRender();
        void RenderText(const std::string& String, v2f pos, float scale = 1.f);

        unsigned int FontTexture;
        float DivAtlasWidth;
        float DivAtlasHeight;
        std::vector<GlyphData> Glyphs;
    };
}
