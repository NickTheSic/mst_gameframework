#include "TextRenderer.h"
#include "mstgl.h"
#include "mstDebug.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <iostream>

namespace mst
{
    TextRenderer::TextRenderer(unsigned int BatchCount, const char* FilePath)
    {
        Init(BatchCount, FilePath, 28);
    }

    TextRenderer::~TextRenderer()
    {
        glDeleteTextures(1, &FontTexture);
        FreeGraphicsMemory();
    }

    void TextRenderer::Init(unsigned int BatchCount, const char* FilePath, int Fontsize)
    {
        InitBaseBufferObjects(BatchCount, sizeof(GlyphVertexData));
        shaderProgram = Shader::GetShader(Shader::Type::FONT);
        InitFontSheet(FilePath, Fontsize);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, pos)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, color)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, size)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, coords)));
        glEnableVertexAttribArray(3);
    }

    void TextRenderer::InitFontSheet(const char* FilePath, int FontSize)
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            *(int*)(0) = 5;
            return;
        }
        
        FT_Face face;
        if (FT_New_Face(ft, FilePath, 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            *(int*)(0) = 5;
            return;
        }
        
        FT_Set_Pixel_Sizes(face, 0, FontSize);
        
        int atlasw = 0;
        int atlash = 0;
        for (unsigned char c = 33; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
        
            atlasw += face->glyph->bitmap.width*4;
            atlash = (face->glyph->bitmap.rows*4 > atlash)
                ? face->glyph->bitmap.rows*4
                : atlash;
        }
        
        DivAtlasWidth = 1.0f / (float)atlasw;
        DivAtlasHeight = 1.0f / (float)atlash;
        
        glGenTextures(1, &FontTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, FontTexture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
        
        int xoffset = 0;
        for (unsigned char c = 33; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            unsigned int* Buffer = new unsigned int[face->glyph->bitmap.width * face->glyph->bitmap.rows];

            for (int i = 0; i < face->glyph->bitmap.width * face->glyph->bitmap.rows; i++)
            {
                Buffer[i] = 0x00ffffff | (face->glyph->bitmap.buffer[i] << 24);
            }
        
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                xoffset, 0,
                face->glyph->bitmap.width, face->glyph->bitmap.rows,
                GL_RGBA,
                GL_UNSIGNED_BYTE, 
                Buffer);//face->glyph->bitmap.buffer);
        
            float tx = (float)xoffset * DivAtlasWidth;
            // now store character for later use
            GlyphData glyph = {
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                tx,
                static_cast<unsigned int>(face->glyph->advance.x >> 6)
            };
            Glyphs.push_back(std::move(glyph));

            delete[] Buffer;
        
            xoffset += face->glyph->bitmap.width*4;
        }
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    void TextRenderer::StartRender()
    {
        glUseProgram(shaderProgram);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBindTexture(GL_TEXTURE_2D, FontTexture);

#if defined __EMSCRIPTEN__ || PLATFORM_WEB
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, pos)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, color)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, size)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, coords)));
        glEnableVertexAttribArray(3);
#endif
    }

    void TextRenderer::EndRender()
    {
        glDrawElements(GL_TRIANGLES, elementDrawCount * 6, GL_UNSIGNED_INT, 0);

        vertexCount = 0;
        elementDrawCount = 0;
    }

    void TextRenderer::RenderText(const std::string& String, v2f pos, float scale /*=1.f*/)
    {
        std::string::const_iterator c;
        for (c = String.begin(); c != String.end(); c++)
        {
            if (vertexCount >= maxVertices)
            {
            #if SLOW_CODE
                doonce(dbglog("Early EndRender in RenderText"));
            #endif
                EndRender();
            }

            if (*c == ' ')
            {
                pos.x += 6 * scale;
                continue;
            }

            const GlyphData& ch = Glyphs[*c - 33];

            float xpos = pos.x + ch.bearing.x * scale;
            float ypos = pos.y - (ch.size.y - ch.bearing.y) * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            float atlasOffsetW = (ch.size.x * DivAtlasWidth);
            float atlasH = (ch.size.y * DivAtlasHeight);

            GlyphVertexData vertices[4] =
            {
                {{xpos,     ypos},     {255,255,255}, ch.size, {ch.xoffset,                atlasH}},
                {{xpos + w, ypos},     {255,255,255}, ch.size, {ch.xoffset + atlasOffsetW, atlasH}},
                {{xpos + w, ypos + h}, {255,255,255}, ch.size, {ch.xoffset + atlasOffsetW, 0.0f}},
                {{xpos,     ypos + h}, {255,255,255}, ch.size, {ch.xoffset,                0.0f}},
            };

            glBufferSubData(GL_ARRAY_BUFFER, vertexCount * sizeof(GlyphVertexData), 4 * sizeof(GlyphVertexData), vertices);

            elementDrawCount++;
            vertexCount += 4;

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            pos.x += (ch.advance * scale); // bitshift by 6 to get value in pixels (2^6 = 64)
        }
    }

    void TextRenderer::RenderTextFromRight(const std::string& String, v2f pos, float scale)
    {
        char c;
        for (c = String.length() - 1; c >= 0; c--)
        {
            if (vertexCount >= maxVertices)
            {
                doonce(dbglog("Early EndRender in font Rendering"));
                EndRender();
            }

            if (String[c] == ' ')
            {
                pos.x -= 6 * scale;
                continue;
            }

            const int idx = String[c] - 33;
            const GlyphData& ch = Glyphs[idx];

            // Reverse by the size of the character first
            pos.x -= (ch.advance) * scale;

            float xpos = pos.x + ch.bearing.x * scale;
            float ypos = pos.y - (ch.size.y - ch.bearing.y) * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            float atlasOffsetW = (ch.size.x * DivAtlasWidth);
            float atlasH = (ch.size.y * DivAtlasHeight);

            GlyphVertexData vertices[4] =
            {
                {{xpos,     ypos},     {255,255,255}, ch.size, {ch.xoffset,                atlasH}},
                {{xpos + w, ypos},     {255,255,255}, ch.size, {ch.xoffset + atlasOffsetW, atlasH}},
                {{xpos + w, ypos + h}, {255,255,255}, ch.size, {ch.xoffset + atlasOffsetW, 0.0f}},
                {{xpos,     ypos + h}, {255,255,255}, ch.size, {ch.xoffset,                0.0f}},
            };

            glBufferSubData(GL_ARRAY_BUFFER, vertexCount * sizeof(GlyphVertexData), 4 * sizeof(GlyphVertexData), vertices);

            elementDrawCount++;
            vertexCount += 4;
        }
    }
}
