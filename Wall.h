#pragma once

#include "Includes.h"
#include <QOpenGLWidget>
#include "Shader.h"

class ModelContainer;

enum Location
{
    LEFT_WALL,
    RIGHT_WALL,
    BACK_WALL,
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

    glm::vec3 color;

    void BufferData() const;

public:
    Wall(QOpenGLWidget* targetWidget, GLfloat distance, GLfloat width, Location location);
    ~Wall();

    // Getter for location
    Location GetLocation() const { return location; }

    // Convert a string to location enumeration
    static Location GetLocationByName(string locationName);

    // Getter and setter for color
    void SetColor(glm::vec3 color) { this->color = color; }
    glm::vec3 GetColor() const { return color; }

    void Draw(glm::mat4 const& view, glm::mat4 const& projection) const;
    void Bind(ModelContainer* container) const;
};

