#pragma once
#include "mstgl.h"
#include "mstglm.h"

#include "RenderUtils.h"

#include <string>
#include <vector>

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
        ~QuadRenderer();
        void Init(unsigned int BatchCount);

        // adds a rect where pos = lower left and size = top right
        void AddRect(const v2f& pos, const v2f& size, const Color& c = Color(255, 255, 255));
        void AddCenteredQuad(const v2f& pos, const v2f& size, const Color& c = Color(255,255,255));

        void StartRender();
        void EndRender();

        RendererData rd;
    };
}
