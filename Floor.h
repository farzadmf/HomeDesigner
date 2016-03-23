#pragma once

#include "Includes.h"
#include <QOpenGLWidget>
#include "Shader.h"

enum FloorRenderMode
{
    FLOOR_COLOR,
    FLOOR_TEXTURE
};

class Floor
{
    QOpenGLWidget* targetWidget = nullptr;
    GLfloat width = 10.0f;

    GLuint floorVao = 0;
    GLuint floorVbo = 0;
    GLuint floorEbo = 0;
    GLuint bottomFloorVao = 0;
    GLuint bottomFloorVbo = 0;
    GLuint bottomFloorEbo = 0;

    unique_ptr<Shader> shader;

    FloorRenderMode renderMode;     // Wether to use color or texture to render
    glm::vec3 color;
    GLuint textureId;

    void BufferData() const;

public:
    explicit Floor(QOpenGLWidget* targetWidget, GLfloat width);
    ~Floor();

    // Getter and setter for color
    void SetColor(glm::vec3 color);
    glm::vec3 GetColor() const { return color; }

    // Getter and setter for texture
    void SetTexture(string textureFilePath);
    GLuint GetTextureId() const { return textureId; }

    void Draw(glm::mat4 const& view, glm::mat4 const& projection) const;
};

