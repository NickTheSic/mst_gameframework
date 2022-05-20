#include "TextRenderer.h"
#include "mstgl.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <iostream>

namespace mst
{
    TextRenderer::TextRenderer(unsigned int BatchCount, const std::string& FileName)
    {
        Init(BatchCount, FileName);

        InitTextShader(rd.shaderProgram);
    }

    TextRenderer::~TextRenderer()
    {
        glDeleteTextures(1, &FontTexture);

        FreeRenderData(rd);
    }

    void TextRenderer::Init(unsigned int BatchCount, const std::string& FileName)
    {
        InitializeRendererData<GlyphVertexData>(rd, BatchCount);
        
        InitFont(FileName);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, pos)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, color)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, size)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, coords)));
        glEnableVertexAttribArray(3);
    }

    void TextRenderer::StartRender()
    {
        glUseProgram(rd.shaderProgram);
        glActiveTexture(GL_TEXTURE0 );
        glBindVertexArray(rd.vao);
        glBindBuffer(GL_ARRAY_BUFFER, rd.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rd.ebo);
        glBindTexture(GL_TEXTURE_2D, FontTexture);

        rd.DrawsPerFrame = 0;
    }

    void TextRenderer::EndRender()
    {
        glDrawElements(GL_TRIANGLES, rd.elementDrawCount * 6, GL_UNSIGNED_INT, 0);

        rd.vertexCount = 0;
        rd.elementDrawCount = 0;

        rd.DrawsPerFrame++;
    }

    void TextRenderer::InitFont(std::string FileName)
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            *(int*)(0) = 5;
            return;
        }
    
        FT_Face face;
        if (FT_New_Face(ft, FileName.c_str(), 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            *(int*)(0) = 5;
            return;
        }
    
        FT_Set_Pixel_Sizes(face, 0, 30);
    
        int atlasw = 0;
        int atlash = 0;
        for (unsigned char c = OffsetChar; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
    
            atlasw += face->glyph->bitmap.width;
            atlash = (face->glyph->bitmap.rows > atlash)
                ? face->glyph->bitmap.rows
                : atlash;
        }
    
        DivAtlasWidth = 1.0f / (float)atlasw;
        DivAtlasHeight = 1.0f / (float)atlash;
    
        glGenTextures(1, &FontTexture);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, FontTexture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            atlasw,
            atlash,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            0
        );
    
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        int xoffset = 0;
        for (unsigned char c = OffsetChar; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
    
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                xoffset, 0,
                face->glyph->bitmap.width, face->glyph->bitmap.rows,
                GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    
            float tx = (float)xoffset * DivAtlasWidth;
            // now store character for later use
            GlyphData glyph = {
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                tx,
                static_cast<unsigned int>(face->glyph->advance.x >> 6)
            };
            Glyphs.push_back(std::move(glyph));

            xoffset += face->glyph->bitmap.width;
        }
    
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
    
    void TextRenderer::RenderString(const std::string& text, v2f& pos)
    {
        //float scale = 1.f;
    
        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            if (rd.vertexCount + 4 >= rd.maxVertices)
            {
                std::cout << "Early EndRender in font Rendering" << std::endl;
                EndRender();
            }
    
            if (*c == ' ')
            {
                pos.x += 6;// * scale;
                continue;
            }
    
            const GlyphData& ch = Glyphs[*c - OffsetChar];
    
            float xpos = pos.x + ch.bearing.x;// * scale;
            float ypos = pos.y - (ch.size.y - ch.bearing.y);// * scale;
    
            float w = ch.size.x;// * scale;
            float h = ch.size.y;// * scale;
    
            float atlasOffsetW = (ch.size.x * DivAtlasWidth);
            float atlasH       = (ch.size.y * DivAtlasHeight);
    
            GlyphVertexData vertices[4] =
            {
                {{xpos,     ypos},     {255,255,255}, ch.size, {ch.xoffset,                atlasH}},
                {{xpos + w, ypos},     {255,255,255}, ch.size, {ch.xoffset + atlasOffsetW, atlasH}},
                {{xpos + w, ypos + h}, {255,255,255}, ch.size, {ch.xoffset + atlasOffsetW, 0.0f}},
                {{xpos,     ypos + h}, {255,255,255}, ch.size, {ch.xoffset,                0.0f}},
            };
    
            glBufferSubData(GL_ARRAY_BUFFER, rd.vertexCount * sizeof(GlyphVertexData), 4 * sizeof(GlyphVertexData), vertices);
    
            rd.elementDrawCount++;
            rd.vertexCount += 4;
    
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            pos.x += (ch.advance);// * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
    }
}
