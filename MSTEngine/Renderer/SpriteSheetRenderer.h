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

    struct SpriteSheetSprite
    {
        v2f bl_coord;
        v2f ur_coord;
        v2f size;
    };

    class SpriteSheetGeneratorRenderer : public BaseRenderer
    {
    public:
        SpriteSheetGeneratorRenderer() = default;
        SpriteSheetGeneratorRenderer(unsigned int BatchCount, std::initializer_list<const char*> FilePath);
        ~SpriteSheetGeneratorRenderer();
        void Init(unsigned int BatchCount, std::initializer_list<const char*> FilePath);
        void Init(unsigned int BatchCount, std::vector<std::string> FilePath);
        void Init(unsigned int BatchCount, const char* const* FilePath, int Paths);
        void GenerateSpriteSheet(const char* const* FilePath, int Paths);
        void InitShader();

        void StartRender();
        void EndRender();

        void RenderFullSheet();
        void RenderSpriteAtIndex(int idx, const v2f& pos);
        void QuickRender(const v2f& pos);
        void QuickRenderSansImage(const v2f& pos);

        std::vector<SpriteSheetSprite> Sprites;

        unsigned int SpriteSheetTexture;
        float DivAtlasWidth;
        float DivAtlasHeight;
    };
}
