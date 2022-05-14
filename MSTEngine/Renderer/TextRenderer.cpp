#include "TextRenderer.h"
#include "mstgl.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include <iostream>

namespace mst
{
    TextRenderer::TextRenderer(unsigned int BatchCount, const std::string& FileName)
    {
        Init(BatchCount, FileName);
    }

    TextRenderer::~TextRenderer()
    {
        for (auto& g : Glyphs)
        {
            glDeleteTextures(1, &g.TextureId);
        }
    }
    void TextRenderer::Init(unsigned int BatchCount, const std::string& FileName)
    {
        rd.maxVertices = BatchCount * 4;

        unsigned int IndiceCount = BatchCount * 6;

        glGenVertexArrays(1, &rd.vao);
        glGenBuffers(1, &rd.vbo);
        glGenBuffers(1, &rd.ebo);

        glBindVertexArray(rd.vao);

        glBindBuffer(GL_ARRAY_BUFFER, rd.vbo);
        glBufferData(GL_ARRAY_BUFFER,
            size_t(4) * size_t(BatchCount) * sizeof(GlyphVertexData),
            NULL, GL_DYNAMIC_DRAW);

        unsigned int* indices = new unsigned int[IndiceCount];
        unsigned int offset = 0;
        for (size_t i = 0; i < IndiceCount; i += 6)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rd.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            size_t(IndiceCount) * sizeof(unsigned int),
            indices, GL_STATIC_DRAW);

        delete[] indices;

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, pos)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, color)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, glyph.size)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertexData), (void*)(offsetof(GlyphVertexData, glyph.bearing)));
        glEnableVertexAttribArray(3);

        InitFont(FileName);
    }

    void TextRenderer::InitFont(std::string FileName)
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }

        FT_Face face;
        if (FT_New_Face(ft, FileName.c_str(), 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }

        FT_Set_Pixel_Sizes(face, 0, 48);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        for (unsigned char c = 48; c < 122; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            GlyphData glyph = {
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x,
                texture
            };
            Glyphs.push_back(glyph);
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    void TextRenderer::StartRender()
    {
        glBindVertexArray(rd.vao);
        glUseProgram(rd.shaderProgram);

        rd.DrawsPerFrame = 0;
    }

    void TextRenderer::EndRender()
    {
        glDrawElements(GL_TRIANGLES, rd.elementDrawCount * 6, GL_UNSIGNED_INT, 0);

        rd.vertexCount = 0;
        rd.elementDrawCount = 0;

        rd.DrawsPerFrame++;
    }

    void TextRenderer::RenderString(const std::string& text, v2f pos)
    {
        //glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(rd.vao);

        float scale = 10.f;

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            GlyphData ch = Glyphs[*c];

            float xpos = pos.x + ch.bearing.x * scale;
            float ypos = pos.y - (ch.size.y - ch.bearing.y) * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureId);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, rd.vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            pos.x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
