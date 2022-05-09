#include "mst_BatchRenderer.h"
#include <array>

namespace mst
{

    QuadRenderer::QuadRenderer(unsigned int BatchCount)
    {
        Init(BatchCount);
    }

    void QuadRenderer::Init(unsigned int BatchCount)
    {
        maxVertices = BatchCount * 4;
        unsigned int IndiceCount = BatchCount * 6;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
            size_t(4) * size_t(BatchCount) * sizeof(VertexData),
            NULL, GL_DYNAMIC_DRAW);

        unsigned int* indices = new unsigned int[IndiceCount];
        unsigned int offset = 0;
        for (size_t i = 0; i < IndiceCount; i+=6)
        {
            indices[i+0] = offset+0;
            indices[i+1] = offset+1;
            indices[i+2] = offset+2;
            indices[i+3] = offset+2;
            indices[i+4] = offset+3;
            indices[i+5] = offset+0;
            offset+=4;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            size_t(IndiceCount) * sizeof(unsigned int),
            indices, GL_STATIC_DRAW);

        delete[] indices;

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(offsetof(VertexData, pos)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData), (void*)(offsetof(VertexData, color)));
        glEnableVertexAttribArray(1);
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

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
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

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
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

        DrawsPerFrame = 0;
    }

    void QuadRenderer::EndRender()
    {
        glDrawElements(GL_TRIANGLES, elementDrawCount * 6, GL_UNSIGNED_INT, 0);

        vertexCount = 0;
        elementDrawCount = 0;

        DrawsPerFrame++;
    }

    void CompileShader(unsigned int& shader, unsigned int type, const char* shaderSource)
    {
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderSource, 0);
        glCompileShader(shader);

        int  success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "Error compiling shader: " << type << std::endl;
            std::cout << infoLog << std::endl;
        }
    }

    void InitColourShader(unsigned int& Program)
    {
        const char* vertexShaderSource = 
            "#version 330 core                                            \n"
            "layout (location = 0) in vec2 aPos;                          \n"
            "layout (location = 1) in vec3 aColour;                       \n"
            "uniform vec2 u_WorldSize;                                    \n"
            "uniform vec2 u_CameraPos;                                    \n"
            "uniform float u_CameraZoom;                                  \n"
            "out vec4 oColour;                                            \n"
            "void main()                                                  \n"
            "{                                                            \n"
            "   oColour = vec4(aColour, u_CameraZoom);                             \n"
            "   vec2 pos = aPos;                                          \n"
            "   pos -= u_CameraPos;                                       \n"
            "   pos /= u_WorldSize * 0.5;                                 \n"
            "   gl_Position = vec4(pos, 0, 1.0);             \n"
            "}                                                            \0";

        const char* fragmentShaderSource = 
            "#version 330 core                                \n"
            "out vec4 FragColor;                              \n"
            "in vec4 oColour;                                 \n"
            "void main(){                                     \n"
            "FragColor = oColour;                             \n"
            "}                                                \0";

        InitShaderCode(Program, vertexShaderSource, fragmentShaderSource);
    }

    void InitShaderCode(unsigned int& Program, const char* vertexShaderSource, const char* fragmentShaderSource)
    {
        unsigned int vertex, fragment;

        CompileShader(vertex, GL_VERTEX_SHADER, vertexShaderSource);
        CompileShader(fragment, GL_FRAGMENT_SHADER, fragmentShaderSource);

        Program = glCreateProgram();
        glAttachShader(Program, vertex);
        glAttachShader(Program, fragment);
        glLinkProgram(Program);

        // check program error
        {
            int  success;
            char infoLog[512];
            glGetProgramiv(Program, GL_LINK_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(Program, 512, NULL, infoLog);
                std::cout << "Error linking Shader Program" << std::endl;
                std::cout << infoLog << std::endl;
            }
        }

        glUseProgram(Program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

}
