#pragma once

#include "mstglm.h"
#include "RenderUtils.h"
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

    class TextRenderer
    {
    public:
        TextRenderer(unsigned int BatchCount, const std::string& FileName);
        ~TextRenderer();
        void Init(unsigned int BatchCount, const std::string& FileName);

        void InitFont(std::string FileName);

        void StartRender();
        void EndRender();

        void RenderString(const std::string& String, v2f& pos);

        RendererData rd;

        unsigned int FontTexture;
        float DivAtlasWidth;
        float DivAtlasHeight;
        unsigned char OffsetChar = 32; // First 32 characters are control codes

        // used to keep track of the loaded glyphs
        std::vector<GlyphData> Glyphs;

        unsigned char* ImageData;
    };
}
