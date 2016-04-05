#pragma once

#include "Includes.h"

using std::string;
using std::ifstream;
///
/// \brief The Shader class
/// 
///
class Shader
{
public:
    ///
    /// \brief Shader sets default shader values
    ///
    Shader();
    ///
    /// \brief Shader read vertex and fragment shaders file path then compile the shaders
    /// \param vertexPath file path to the vertex shader
    /// \param fragmentPath file path to the fragment shader
    ///
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    ///
    /// \brief Shader Shader read vertex and fragment shaders file path then compile the shaders as well as reading the geometry
    /// \param vertexPath file path to the vertex shader
    /// \param fragmentPath file path to the fragment shader
    /// \param geometryPath file path to geometry shader
    ///
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);

    GLuint GetProgram() const { return program; }
    ///
    /// \brief Create generates the shader program
    ///
    void Create();
    ///
    /// \brief Use uses the shader program
    ///
    void Use() const;

private:
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint geometryShader;
};
