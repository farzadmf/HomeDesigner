#pragma once

#include "Includes.h"
#include <memory>
#include <vector>
#include "Wall.h"

class ModelContainer;
class Floor;
class QOpenGLWidget;

class Room
{
    GLfloat width;

    QOpenGLWidget* targetWidget = nullptr;
    unique_ptr<Floor> floor;
    vector<shared_ptr<Wall>> walls;

public:
    Room(QOpenGLWidget* targetWidget, GLfloat roomWidth);
    ~Room();

    Floor* GetFloor() const { return floor.get(); }
    vector<shared_ptr<Wall>> GetWalls() const { return walls; }

    glm::vec3 GetMinimumVertices() const;
    glm::vec3 GetMaximumVertices() const;

    void Draw(glm::mat4 const& view, glm::mat4 const& projection) const;
    void BindToFloor(ModelContainer* container);
    void BindToWall(ModelContainer* container, Location wallLocation);
};

