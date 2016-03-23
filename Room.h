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

    // Color for wall and floor
    glm::vec3 wallColor, floorColor;

    // Texture file path for wall and floor
    string wallTexture, floorTexture;

public:
    Room(QOpenGLWidget* targetWidget, GLfloat roomWidth, glm::vec3 wallColor, glm::vec3 floorColor);
    ~Room();

    Floor* GetFloor() const { return floor.get(); }
    vector<shared_ptr<Wall>> GetWalls() const { return walls; }

    // Getters and setters for colors
    void SetWallColor(glm::vec3 wallColor);
    glm::vec3 GetWallColor() const { return wallColor; }
    void SetFloorColor(glm::vec3 floorColor);
    glm::vec3 GetFloorColor() const { return floorColor; }

    // Getters and setters for textures
    void SetWallTexture(string wallTexture);
    string GetWallTexture() const { return wallTexture; }
    void SetFloorTexture(string floorTexture);
    string GetFloorTexture() const { return floorTexture; }

    glm::vec3 GetMinimumVertices() const;
    glm::vec3 GetMaximumVertices() const;

    void Draw(glm::mat4 const& view, glm::mat4 const& projection) const;
    void BindToFloor(ModelContainer* container);
    void BindToWall(ModelContainer* container, Location wallLocation);
};

