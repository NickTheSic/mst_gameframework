#include "QuadRenderer.h"
#include "mstgl.h"
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
        FreeGraphicsMemory();
    }

    void QuadRenderer::Init(unsigned int BatchCount)
    {
        InitBaseBufferObjects(BatchCount, sizeof(VertexData));
        InitColourShader();

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(offsetof(VertexData, pos)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData), (void*)(offsetof(VertexData, color)));
        glEnableVertexAttribArray(1);
    }

    void QuadRenderer::InitColourShader()
    {
        const char* vertexShaderSource =
#if defined PLATFORM_WEB | defined __EMSCRIPTEN__
            "#version 300 es                                              \n"
            "precision mediump float;                                     \n"
#else
            "#version 330 core                                            \n"
#endif
            "layout (location = 0) in vec2 aPos;                          \n"
            "layout (location = 1) in vec3 aColour;                       \n"
            "uniform vec2 u_WorldSize;                                    \n"
            "uniform vec2 u_CameraPos;                                    \n"
            "uniform float u_CameraZoom;                                  \n"
            "out vec3 oColour;                                            \n"
            "void main()                                                  \n"
            "{                                                            \n"
            "   oColour = aColour;                                        \n"
            "   vec2 pos = aPos;                                          \n"
            "   pos -= u_CameraPos;                                       \n"
            "   pos /= u_WorldSize * 0.5;                                 \n"
            "   gl_Position = vec4(pos, 0, 1.0);                          \n"
            "}                                                            \0";

        const char* fragmentShaderSource =
#if defined PLATFORM_WEB | defined __EMSCRIPTEN__
            "#version 300 es                                              \n"
            "precision mediump float;                                     \n"
#else
            "#version 330 core                                            \n"
#endif
            "out vec4 FragColor;                                          \n"
            "in vec3 oColour;                                             \n"
            "void main(){                                                 \n"
            "FragColor = vec4(oColour, 1.0);                              \n"
            "}                                                            \0";

        CompileShaderProgram(vertexShaderSource, fragmentShaderSource);
    }

    void QuadRenderer::AddRect(const v2f& pos, const v2f& size, const Color& c)
    {
        if (vertexCount + 4 > maxVertices)
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
            vertexCount * sizeof(VertexData),
            4 * sizeof(VertexData),
            &vertices[0]);

        elementDrawCount++;
        vertexCount+=4;
    }

    void QuadRenderer::AddCenteredQuad(const v2f& pos, const v2f& size, const Color& c)
    {
        if (vertexCount + 4 > maxVertices)
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
            vertexCount * sizeof(VertexData),
            4 * sizeof(VertexData),
            &vertices[0]);

        elementDrawCount++;
        vertexCount += 4;
    }

    void QuadRenderer::StartRender()
    {
        glBindVertexArray(vao);
        glUseProgram(shaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

    }

    void QuadRenderer::EndRender()
    {
        glDrawElements(GL_TRIANGLES, elementDrawCount * 6, GL_UNSIGNED_INT, 0);

        vertexCount = 0;
        elementDrawCount = 0;
    }
}
