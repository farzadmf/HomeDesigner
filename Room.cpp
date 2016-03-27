#include "Shader.h"
#include "Room.h"
#include "Floor.h"
#include "ModelContainer.h"

using std::make_unique;

/**
* Create the room with specified width, and 
**/
Room::Room(QOpenGLWidget* targetWidget, GLfloat roomWidth, glm::vec3 wallColor, glm::vec3 floorColor) :
    width{ roomWidth }, wallColor{ wallColor }, floorColor{ floorColor }
{
    this->targetWidget = targetWidget;

    // Create the floor
    floor = make_unique<Floor>(targetWidget, width);
    floor->SetColor(this->floorColor);

    // Create the walls
    auto leftWall = make_shared<Wall>(targetWidget, width / 2.0f, width, RIGHT_WALL, WALL_COLOR);
    leftWall->SetColor(this->wallColor);
    auto rightWall = make_shared<Wall>(targetWidget, width / 2.0f, width, LEFT_WALL, WALL_COLOR);
    rightWall->SetColor(this->wallColor);
    auto backWall = make_shared<Wall>(targetWidget, width / 2.0f, width, BACK_WALL, WALL_COLOR);
    backWall->SetColor(this->wallColor);

    walls.push_back(leftWall);
    walls.push_back(rightWall);
    walls.push_back(backWall);
}

Room::~Room()
{
}

/**
* Changes the color of the walls
**/
void Room::SetWallColor(glm::vec3 wallColor)
{
    this->wallColor = wallColor;
    for (auto wall : walls)
        wall->SetColor(wallColor);
}

/**
* Changes the color of the floor
**/
void Room::SetFloorColor(glm::vec3 floorColor)
{
    this->floorColor = floorColor;
    floor->SetColor(floorColor);
}

/**
* Changes the texture of the walls
**/
void Room::SetWallTexture(string wallTexture)
{
    this->wallTexture = wallTexture;
    for (auto wall : walls)
        wall->SetTexture(this->wallTexture);
}

/**
* Changes the texture of the floor
**/
void Room::SetFloorTexture(string floorTexture)
{
    this->floorTexture = floorTexture;
    floor->SetTexture(this->floorTexture);
}

/**
* Returns back the minimum coordinates of the room
**/
glm::vec3 Room::GetMinimumCoordinates() const
{
    return glm::vec3(-width / 2.0f, 0.0f, -width / 2.0f);
}

/**
* Returns back the maximum coordinates of the room
**/
glm::vec3 Room::GetMaximumCoordinates() const
{
    return glm::vec3(width / 2.0f, width / 2.0f, width / 2.0f);
}

/**
* Draws the room (floor and the walls)
**/
void Room::Draw(glm::mat4 const& view, glm::mat4 const& projection) const
{
    // Don't write to the stencil buffer
    glStencilMask(0x00);

    for (auto wall : walls)
        wall->Draw(view, projection);

    floor->Draw(view, projection);
}

/**
* Binds the specified model container to the floor
**/
void Room::BindToFloor(ModelContainer* container)
{
    // Set the rotation bound to be 'y' axis, and the translation bound to be 'xz'
    container->SetRotationBound(glm::vec3(0.0f, 1.0f, 0.0f));
    container->SetTranslationBound(glm::vec3(1.0f, 0.0f, 1.0f));

    // Just a small offset from the ground
    container->SetInitialTranslateVector(glm::vec3(0.0f, 0.001f, 0.0f));
}

/**
* Binds the specified model container to the specified wall
**/
void Room::BindToWall(ModelContainer* container, Location wallLocation)
{
    for (auto wall : walls)
        if (wall->GetLocation() == wallLocation)
            wall->Bind(container);
}
