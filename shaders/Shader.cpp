#include "Shader.h"
#include "MyHelpers.h"

Shader::Shader() : program(0), vertexShader(0), fragmentShader(0), geometryShader(0) { }

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    : Shader()
{
    string vertexCode;
    string fragmentCode;
    ifstream vertexShaderFile;
    ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(ifstream::badbit);
    fragmentShaderFile.exceptions(ifstream::badbit);

    try
    {
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);

        std::stringstream vertexShaderStream, fragmentShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    }
    catch (ifstream::failure e)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }

    const GLchar* vertexShaderCode = vertexCode.c_str();
    const GLchar* fragmentShaderCode = fragmentCode.c_str();

    vertexShader = generateVertexShader(vertexShaderCode);
    fragmentShader = generateFragmentShader(fragmentShaderCode);
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
    : Shader(vertexPath, fragmentPath)
{
    string geometryCode;
    ifstream geometryShaderFile;

    geometryShaderFile.exceptions(ifstream::badbit);

    try
    {
        geometryShaderFile.open(geometryPath);
        std::stringstream vertexShaderStream, fragmentShaderStream;
        vertexShaderStream << geometryShaderFile.rdbuf();
        geometryShaderFile.close();
        geometryCode = vertexShaderStream.str();
    }
    catch (ifstream::failure e)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }

    const GLchar* geometryShaderCode = geometryCode.c_str();

    geometryShader = GenerateGeometryShader(geometryShaderCode);
}

void Shader::Create()
{
    program = GenerateShaderProgram(vertexShader, fragmentShader, geometryShader);
}

void Shader::Use() const
{
    glUseProgram(program);
}
