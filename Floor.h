#pragma once

#include "Includes.h"
#include <QOpenGLWidget>
#include "Shader.h"
///
/// \brief The FloorRenderMode enum for the color and texture
///
enum FloorRenderMode
{
    FLOOR_COLOR,
    FLOOR_TEXTURE
};
///
/// \brief The Floor class
/// allows the adding of colors and texture to the floor using shaders.
///
class Floor
{
    QOpenGLWidget* targetWidget = nullptr;
    GLfloat width = 10.0f;

    GLuint floorVao       = 0;
    GLuint floorVbo       = 0;
    GLuint floorEbo       = 0;
    GLuint bottomFloorVao = 0;
    GLuint bottomFloorVbo = 0;
    GLuint bottomFloorEbo = 0;

    unique_ptr<Shader> shader;

    FloorRenderMode renderMode;     // Wether to use color or texture to render
    glm::vec3 color;
    GLuint textureId;

    void BufferData() const;

public:
    ///
    /// \brief Floor creates the array buffers and the shader
    /// \param targetWidget target width
    /// \param width the width in floating point value
    ///
    explicit Floor(QOpenGLWidget* targetWidget, GLfloat width);
    ~Floor();

  
    ///
    /// \brief SetColor
    /// \param color
    ///
    void SetColor(glm::vec3 color);
    ///
    /// \brief GetColor
    /// \return
    ///
    glm::vec3 GetColor() const { return color; }

    ///
    /// \brief SetTexture setter for texture
    /// \param textureFilePath
    ///
    void SetTexture(string textureFilePath);
    ///
    /// \brief GetTextureId getter for texture
    /// \return
    ///
    GLuint GetTextureId() const { return textureId; }
    ///
    /// \brief Draw sets the color texture of the bottom and main floors
    /// \param view view matrix
    /// \param projection projection matrix
    ///
    void Draw(glm::mat4 const& view, glm::mat4 const& projection) const;
};

