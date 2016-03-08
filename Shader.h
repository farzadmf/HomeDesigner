#pragma once

#include "Includes.h"

using std::string;
using std::ifstream;

class Shader
{
public:
    Shader();
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);

    GLuint GetProgram() const { return program; }
    void Create();

    void Use() const;

private:
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint geometryShader;
};
