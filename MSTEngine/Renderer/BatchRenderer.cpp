#include "BatchRenderer.h"
#include <array>
#include <iostream>

namespace mst
{
    QuadRenderer::QuadRenderer(unsigned int BatchCount)
    {
        Init(BatchCount);
    }

    QuadRenderer::~QuadRenderer()
    {
        FreeRenderData(rd);
    }

    void QuadRenderer::Init(unsigned int BatchCount)
    {
        InitializeRendererData<VertexData>(rd, BatchCount);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(offsetof(VertexData, pos)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData), (void*)(offsetof(VertexData, color)));
        glEnableVertexAttribArray(1);
    }

    void QuadRenderer::AddRect(const v2f& pos, const v2f& size, const Color& c)
    {
        if (rd.vertexCount + 4 > rd.maxVertices)
        {
            EndRender();
        }

        std::array<VertexData, 4> vertices;

        vertices[0].pos.x = pos.x;
        vertices[0].pos.y = pos.y;
        vertices[0].color = c;
       
        vertices[1].pos.x = pos.x + size.x;
        vertices[1].pos.y = pos.y;
        vertices[1].color = c;
       
        vertices[2].pos.x = pos.x + size.x;
        vertices[2].pos.y = pos.y + size.y;
        vertices[2].color = c;
        
        vertices[3].pos.x = pos.x;
        vertices[3].pos.y = pos.y + size.y;
        vertices[3].color = c;

        glBufferSubData(GL_ARRAY_BUFFER,
            rd.vertexCount * sizeof(VertexData),
            4 * sizeof(VertexData),
            &vertices[0]);

        rd.elementDrawCount++;
        rd.vertexCount+=4;
    }

    void QuadRenderer::AddCenteredQuad(const v2f& pos, const v2f& size, const Color& c)
    {
        if (rd.vertexCount + 4 > rd.maxVertices)
        {
            EndRender();
        }

        v2f hsize(size*0.5f);

        std::array<VertexData, 4> vertices;

        vertices[0].pos = pos - hsize;
        vertices[0].color = c;

        vertices[1].pos.x = pos.x + hsize.x;
        vertices[1].pos.y = pos.y - hsize.y;
        vertices[1].color = c;

        vertices[2].pos = pos + hsize;
        vertices[2].color = c;

        vertices[3].pos.x = pos.x - hsize.y;
        vertices[3].pos.y = pos.y + hsize.y;
        vertices[3].color = c;

        glBufferSubData(GL_ARRAY_BUFFER,
            rd.vertexCount * sizeof(VertexData),
            4 * sizeof(VertexData),
            &vertices[0]);

        rd.elementDrawCount++;
        rd.vertexCount += 4;
    }

    void QuadRenderer::StartRender()
    {
        glBindVertexArray(rd.vao);
        glUseProgram(rd.shaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, rd.vbo);

        rd.DrawsPerFrame = 0;
    }

    void QuadRenderer::EndRender()
    {
        glDrawElements(GL_TRIANGLES, rd.elementDrawCount * 6, GL_UNSIGNED_INT, 0);

        rd.vertexCount = 0;
        rd.elementDrawCount = 0;

        rd.DrawsPerFrame++;
    }
}
