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

    void BufferData() const;

public:
    explicit Floor(QOpenGLWidget* targetWidget, GLfloat width);
    ~Floor();

    // Getter and setter for color
    void SetColor(glm::vec3 color) { this->color = color; }
    glm::vec3 GetColor() const { return color; }

    // Getter and setter for render mode
    void SetRenderMode(FloorRenderMode renderMode) { this->renderMode = renderMode; }
    FloorRenderMode GetRenderMode() const { return renderMode; }

    void Draw(glm::mat4 const& view, glm::mat4 const& projection) const;
};

