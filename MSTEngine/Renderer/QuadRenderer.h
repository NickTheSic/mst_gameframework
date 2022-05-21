#pragma once
#include "mstglm.h"
#include "BaseRenderer.h"
#include <string>
#include <vector>

namespace mst
{
    struct VertexData
    {
        v2f pos;
        Color color;
    };

    class QuadRenderer : public BaseRenderer
    {
    public:
        QuadRenderer(unsigned int BatchCount);
        ~QuadRenderer();
        void Init(unsigned int BatchCount);

        void InitColourShader();

        // adds a rect where pos = lower left and size = top right
        void AddRect(const v2f& pos, const v2f& size, const Color& c = Color(255, 255, 255));
        void AddCenteredQuad(const v2f& pos, const v2f& size, const Color& c = Color(255,255,255));

        void StartRender();
        void EndRender();
    };
}
