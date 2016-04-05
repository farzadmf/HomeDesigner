#pragma once

#include "Includes.h"
#include <memory>
#include <vector>
#include "Wall.h"

class ModelContainer;
class Floor;
class QOpenGLWidget;
///
/// \brief The Room class 
/// Creates a room with 3 walls and sets the color and texture of the room
///
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
    ///
    /// \brief Room Creates the room with specific widths and colors
    /// \param targetWidget target width
    /// \param roomWidth room width
    /// \param wallColor wall color
    /// \param floorColor floor color
    ///
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

    glm::vec3 GetMinimumCoordinates() const;
    glm::vec3 GetMaximumCoordinates() const;
    ///
    /// \brief Draw Draws the room floor and walls
    /// \param view view matrix
    /// \param projection projection matrix
    ///
    void Draw(glm::mat4 const& view, glm::mat4 const& projection) const;
    ///
    /// \brief BindToFloor binds the specific model container to the floor y axis and allows movement in xz plane
    /// \param container
    ///
    void BindToFloor(ModelContainer* container);
    ///
    /// \brief BindToWall binds the specific model container to the wall
    /// \param container
    /// \param wallLocation
    ///
    void BindToWall(ModelContainer* container, Location wallLocation);
    bool IsAttached(ModelContainer* container, Location wallLocation);
};

