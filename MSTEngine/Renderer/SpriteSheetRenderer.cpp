#include "SpriteSheetRenderer.h"
#include "mstgl.h"
#include "mstDebug.h"
#include <stb_image.h>
#include <iostream>
#include <array>

namespace mst
{
    SpriteSheetGeneratorRenderer::SpriteSheetGeneratorRenderer(unsigned int BatchCount, std::initializer_list<const char*> FilePath)
    {
        Init(BatchCount, FilePath);
    }

    SpriteSheetGeneratorRenderer::~SpriteSheetGeneratorRenderer()
    {
        glDeleteTextures(1, &SpriteSheetTexture);
        FreeGraphicsMemory();
    }

    void SpriteSheetGeneratorRenderer::Init(unsigned int BatchCount, std::initializer_list<const char*> FilePath)
    {
        Init(BatchCount, FilePath.begin(), FilePath.size());
    }

    void SpriteSheetGeneratorRenderer::Init(unsigned int BatchCount, std::vector<std::string> FilePath)
    {
        std::vector<const char*> strings;
        strings.reserve(FilePath.size());
        for (auto& str : FilePath)
        {
            strings.push_back(str.c_str());
        }
        Init(BatchCount, strings.data(), strings.size());
    }

    void SpriteSheetGeneratorRenderer::Init(unsigned int BatchCount, const char* const* FilePath, int Paths)
    {
        InitBaseBufferObjects(BatchCount, sizeof(SpriteSheetVertexData));
        InitShader();
        GenerateSpriteSheet(FilePath, Paths);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteSheetVertexData), (void*)(offsetof(SpriteSheetVertexData, pos)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteSheetVertexData), (void*)(offsetof(SpriteSheetVertexData, coords)));
        glEnableVertexAttribArray(1);
    }

    void SpriteSheetGeneratorRenderer::GenerateSpriteSheet(const char* const* FilePaths, int Paths)
    {
        std::vector<GeneratedSpriteSheetData> GSD;
        GSD.reserve(Paths);

        dbgval(Paths);

        int atlasw = 0;
        int atlash = 0;

        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &SpriteSheetTexture);
        glBindTexture(GL_TEXTURE_2D, SpriteSheetTexture);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_set_flip_vertically_on_load(true);
        for (int i = 0; i < Paths; i++)
        {
            const char* str = FilePaths[i];
            GeneratedSpriteSheetData gsd = {};

            gsd.data = stbi_load(str, &gsd.x, &gsd.y, &gsd.channel, 0);

            if (stbi_failure_reason())
            { 
                std::cout << stbi_failure_reason();
                continue;
            }
            
           atlasw += gsd.x;
           atlash = (gsd.y > atlash)
               ? gsd.y
               : atlash;
           
           GSD.push_back(gsd);
        }

        if (GSD.size() == 0) { return; };
        if (GSD[0].channel == 3)
        {
            SpriteSheetFormat = GL_RGB;
        }
        else if (GSD[0].channel == 4)
        {
            SpriteSheetFormat = GL_RGBA;
        }
        else
        {
            dbglog("Channel was not 3 or 4");
        }

        DivAtlasWidth = 1.0f / (float)atlasw;
        DivAtlasHeight = 1.0f / (float)atlash;

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            SpriteSheetFormat,
            atlasw,
            atlash,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
#if !defined __EMSCRIPTEN__ || !defined PLATFORM_WEB
            0
#else
            new unsigned char[atlasw*atlash]
#endif
        );
        
        int xoffset = 0;
        for (auto& gsd : GSD)
        {        
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                xoffset, 0,
                gsd.x, gsd.y, 
                SpriteSheetFormat,
                GL_UNSIGNED_BYTE,
                gsd.data);

            xoffset += gsd.x;

            stbi_image_free(gsd.data);
        }
    }

    void SpriteSheetGeneratorRenderer::InitShader()
    {
        const char* vertexShaderSource =
#if defined PLATFORM_WEB || defined __EMSCRIPTEN__
            "#version 300 es                                                 \n"
            "precision mediump float;                                        \n"
#else
            "#version 330 core                                               \n"
#endif
            "layout (location = 0) in vec2 aPos;                             \n"
            "layout (location = 1) in vec2 InCoords;                         \n"
            "out vec2 TexCoords;                                             \n"
            "uniform vec2 u_WorldSize;                                       \n"
            "uniform vec2 u_CameraPos;                                       \n"
            "void main()                                                     \n"
            "{                                                               \n"
            "   vec2 pos = aPos;                                             \n"
            "   pos -= u_CameraPos;                                          \n"
            "   pos /= u_WorldSize * 0.5;                                    \n"
            "   gl_Position = vec4(pos, 0, 1.0);                             \n"
            "   TexCoords = InCoords;                                        \n"
            "}                                                               \0";

        const char* fragmentShaderSource =
#if defined PLATFORM_WEB || defined __EMSCRIPTEN__
            "#version 300 es                                                 \n"
            "precision mediump float;                                        \n"
#else
            "#version 330 core                                               \n"
#endif
            "out vec4 FragColor;                                             \n"
            "in vec2 TexCoords;                                              \n"
            "uniform sampler2D text;                                         \n"
            "void main(){                                                    \n"
            "FragColor = vec4(texture(text, TexCoords));                     \n"
            "}                                                               \0";

        CompileShaderProgram(vertexShaderSource, fragmentShaderSource);
    }

    void SpriteSheetGeneratorRenderer::StartRender()
    {
        glUseProgram(shaderProgram);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBindTexture(GL_TEXTURE_2D, SpriteSheetTexture);

#if defined __EMSCRIPTEN__ || PLATFORM_WEB
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteSheetVertexData), (void*)(offsetof(SpriteSheetVertexData, pos)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteSheetVertexData), (void*)(offsetof(SpriteSheetVertexData, coords)));
        glEnableVertexAttribArray(1);
#endif
    }

    void SpriteSheetGeneratorRenderer::EndRender()
    {
        glDrawElements(GL_TRIANGLES, elementDrawCount * 6, GL_UNSIGNED_INT, 0);

        vertexCount = 0;
        elementDrawCount = 0;
    }

    void SpriteSheetGeneratorRenderer::RenderSpriteAtIndex(int idx, const v2f& pos)
    {
        if (vertexCount + 4 > maxVertices)
        {
            EndRender();
        }

        v2f size = {18, 18};

        std::array<SpriteSheetVertexData, 4> vertices;

        vertices[0].pos.x = pos.x;
        vertices[0].pos.y = pos.y;

        vertices[1].pos.x = pos.x + size.x;
        vertices[1].pos.y = pos.y;

        vertices[2].pos.x = pos.x + size.x;
        vertices[2].pos.y = pos.y + size.y;

        vertices[3].pos.x = pos.x;
        vertices[3].pos.y = pos.y + size.y;

        float atlas_offset = 18 * idx * DivAtlasWidth;

        vertices[0].coords = v2f(atlas_offset, 0.0f);
        vertices[1].coords = v2f(atlas_offset + DivAtlasWidth, 0.0f);
        vertices[2].coords = v2f(atlas_offset + DivAtlasWidth, 1.0f);
        vertices[3].coords = v2f(atlas_offset, 1.0f);

        glBufferSubData(GL_ARRAY_BUFFER,
            vertexCount * sizeof(SpriteSheetVertexData),
            4 * sizeof(SpriteSheetVertexData),
            &vertices[0]);

        elementDrawCount++;
        vertexCount += 4;
    }

    void SpriteSheetGeneratorRenderer::QuickRender(const v2f& pos)
    {
        if (vertexCount + 4 > maxVertices)
        {
            EndRender();
        }

        v2f size = {18*3, 18*3};

        std::array<SpriteSheetVertexData, 4> vertices;

        vertices[0].pos.x = pos.x;
        vertices[0].pos.y = pos.y;

        vertices[1].pos.x = pos.x + size.x;
        vertices[1].pos.y = pos.y;

        vertices[2].pos.x = pos.x + size.x;
        vertices[2].pos.y = pos.y + size.y;

        vertices[3].pos.x = pos.x;
        vertices[3].pos.y = pos.y + size.y;

        static float atlas_offset;
        static float quicktimer;

        quicktimer+=0.1;
        if (quicktimer>5)
        {
            atlas_offset += 18 * DivAtlasWidth;
            if (atlas_offset > 1.0f) { atlas_offset -= 1.0f; }
            quicktimer = 0;
        }

        vertices[0].coords = v2f(atlas_offset, 0.0f);
        vertices[1].coords = v2f(atlas_offset+DivAtlasWidth, 0.0f);
        vertices[2].coords = v2f(atlas_offset+DivAtlasWidth, 1.0f);
        vertices[3].coords = v2f(atlas_offset, 1.0f);

        glBufferSubData(GL_ARRAY_BUFFER,
            vertexCount * sizeof(SpriteSheetVertexData),
            4 * sizeof(SpriteSheetVertexData),
            &vertices[0]);

        elementDrawCount++;
        vertexCount += 4;
    }
    void SpriteSheetGeneratorRenderer::QuickRenderSansImage(const v2f& pos)
    {
        if (vertexCount + 4 > maxVertices)
        {
            EndRender();
        }

        v2f size = { 16 * 13, 16 * 2 };

        std::array<SpriteSheetVertexData, 4> vertices;

        vertices[0].pos.x = pos.x;
        vertices[0].pos.y = pos.y;

        vertices[1].pos.x = pos.x + size.x;
        vertices[1].pos.y = pos.y;

        vertices[2].pos.x = pos.x + size.x;
        vertices[2].pos.y = pos.y + size.y;

        vertices[3].pos.x = pos.x;
        vertices[3].pos.y = pos.y + size.y;

        vertices[0].coords = v2f(.5f, 0.5f);
        vertices[1].coords = v2f(.5f, 0.5f);
        vertices[2].coords = v2f(.5f, 0.5f);
        vertices[3].coords = v2f(.5f, 0.5f);

        glBufferSubData(GL_ARRAY_BUFFER,
            vertexCount * sizeof(SpriteSheetVertexData),
            4 * sizeof(SpriteSheetVertexData),
            &vertices[0]);

        elementDrawCount++;
        vertexCount += 4;
    }
}
