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
        InitBaseBufferObjects(BatchCount, sizeof(SpriteSheetVertexData));
        InitShader();
        GenerateSpriteSheet(FilePath);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteSheetVertexData), (void*)(offsetof(SpriteSheetVertexData, pos)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteSheetVertexData), (void*)(offsetof(SpriteSheetVertexData, coords)));
        glEnableVertexAttribArray(1);
    }

    void SpriteSheetGeneratorRenderer::GenerateSpriteSheet(std::initializer_list<const char*> FilePaths)
    {
        std::vector<GeneratedSpriteSheetData> GSD;

        int atlasw = 0;
        int atlash = 0;

        glGenTextures(1, &SpriteSheetTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, SpriteSheetTexture);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_set_flip_vertically_on_load(true);
        for (const char* str : FilePaths)
        {
            GeneratedSpriteSheetData gsd = {};

            gsd.data = stbi_load(str, &gsd.x, &gsd.y, &gsd.channel, 0);

            if (stbi_failure_reason())
            { 
                std::cout << stbi_failure_reason();
                *(int*)0 = 5;
            }

            //if (gsd.data)
            //{
            //    if (gsd.channel == 3)
            //    {
            //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gsd.x, gsd.y, 0, GL_RGB, GL_UNSIGNED_BYTE, gsd.data);
            //    }
            //    else if (gsd.channel == 4)
            //    {
            //        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gsd.x, gsd.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, gsd.data);
            //    }
            //
            //    glGenerateMipmap(GL_TEXTURE_2D);
            //}
            
           atlasw += gsd.x;
           atlash = (gsd.y > atlash)
               ? gsd.y
               : atlash;
           
           GSD.push_back(gsd);
        }
        
        DivAtlasWidth = 1.0f / (float)atlasw;
        DivAtlasHeight = 1.0f / (float)atlash;
        
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
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

        glGenerateMipmap(GL_TEXTURE_2D);
        
        int xoffset = 0;
        for (auto& gsd : GSD)
        {        
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                xoffset, 0,
                gsd.x, gsd.y, 
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                gsd.data);
        
            //float tx = (float)xoffset * DivAtlasWidth;
           //// now store character for later use
           //GlyphData glyph = {
           //    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
           //    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
           //    tx,
           //    static_cast<unsigned int>(face->glyph->advance.x >> 6)
           //};
           //Glyphs.push_back(std::move(glyph));
        
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
            "if (TexCoords == vec2(0.5,0.5)) FragColor = vec4(0,0,0,1);  else \n"
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

    void SpriteSheetGeneratorRenderer::QuickRender(const v2f& pos)
    {
        if (vertexCount + 4 > maxVertices)
        {
            EndRender();
        }

        v2f size = {160*4,96*2};

        std::array<SpriteSheetVertexData, 4> vertices;

        vertices[0].pos.x = pos.x;
        vertices[0].pos.y = pos.y;

        vertices[1].pos.x = pos.x + size.x;
        vertices[1].pos.y = pos.y;

        vertices[2].pos.x = pos.x + size.x;
        vertices[2].pos.y = pos.y + size.y;

        vertices[3].pos.x = pos.x;
        vertices[3].pos.y = pos.y + size.y;

        vertices[0].coords = v2f(0.0f, 0.0f);
        vertices[1].coords = v2f(1.0f, 0.0f);
        vertices[2].coords = v2f(1.0f, 1.0f);
        vertices[3].coords = v2f(0.0f, 1.0f);

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

        v2f size = { 160 * 4,96 * 2 };

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
