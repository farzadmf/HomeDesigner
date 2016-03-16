#pragma once

#include "Includes.h"
#include <QOpenGLWidget>
#include "Shader.h"

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

    void BufferData() const;

public:
    explicit Floor(QOpenGLWidget* targetWidget, GLfloat width);
    ~Floor();

    void Draw(glm::mat4 const& view, glm::mat4 const& projection) const;
};

