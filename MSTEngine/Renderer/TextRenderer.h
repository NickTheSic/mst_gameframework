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
        unsigned int advance;
        unsigned int TextureId;
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

        void RenderString(const std::string& String, v2f pos);
        void RenderChar(unsigned char c);

        RendererData rd;

        unsigned char OffsetChar = '!';

        // used to keep track of the loaded glyphs
        std::vector<GlyphData> Glyphs;
    };
}
