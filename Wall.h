#pragma once

#include "Includes.h"
#include <QOpenGLWidget>
#include "Shader.h"

enum Location
{
    LEFT,
    RIGHT,
    TOP,
};

class Wall
{
    GLfloat distance;
    GLfloat width;
    Location location;

    QOpenGLWidget* targetWidget = nullptr;
    GLuint wallVao = 0;
    GLuint wallVbo = 0;
    GLuint wallEbo = 0;
    GLuint secondWallVao = 0;
    GLuint secondWallVbo = 0;
    GLuint secondWallEbo = 0;

    unique_ptr<Shader> shader;

    void BufferData() const;

public:
    Wall(QOpenGLWidget* targetWidget, GLfloat distance, GLfloat width, Location location);
    ~Wall();

    void Draw(glm::mat4 const& view, glm::mat4 const& projection) const;
};

