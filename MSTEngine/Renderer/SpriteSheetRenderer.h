#pragma once

#include "mstglm.h"
#include "BaseRenderer.h"
#include <initializer_list>
#include <string>
#include <vector>

namespace mst
{
    struct GeneratedSpriteSheetData
    {
        int x, y, channel;
        unsigned char* data = nullptr;
    };
    
    struct SpriteSheetVertexData
    {
        v2f pos;
        v2f coords;
    };

    class SpriteSheetGeneratorRenderer : public BaseRenderer
    {
    public:
        SpriteSheetGeneratorRenderer() = default;
        SpriteSheetGeneratorRenderer(unsigned int BatchCount, std::initializer_list<const char*> FilePath);
        ~SpriteSheetGeneratorRenderer();
        void Init(unsigned int BatchCount, std::initializer_list<const char*> FilePath);
        void GenerateSpriteSheet(std::initializer_list<const char*> FilePath);
        void InitShader();

        void StartRender();
        void EndRender();

        void QuickRender(const v2f& pos);
        void QuickRenderSansImage(const v2f& pos);

        unsigned int SpriteSheetTexture;
        float DivAtlasWidth;
        float DivAtlasHeight;
    };
}
