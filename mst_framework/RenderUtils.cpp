#include "RenderUtils.h"
#include "mstgl.h"
#include <iostream>

namespace mst
{

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
            "   oColour = vec4(aColour, 0);                               \n"
            "   vec2 pos = aPos;                                          \n"
            "   pos -= u_CameraPos;                                       \n"
            "   pos /= u_WorldSize * 0.5;                  \n"
            "   gl_Position = vec4(pos, 0, 1.0);                          \n"
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

    void InitTextShader(unsigned int& Program)
    {
        const char* vertexShaderSource =
            "#version 330 core                                            \n"
            "layout (location = 0) in vec2 aPos;                          \n"
            "layout (location = 1) in vec3 aColour;                       \n"
            "layout (location = 2) in vec2 size;                          \n"
            "out vec2 TexCoords;                                          \n"
            "uniform mat4 projection;                                     \n"
            "void main()                                                  \n"
            "{                                                            \n"
            "   gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);     \n"
            "    TexCoords = vertex.zw;                                   \n"
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