#include "Shader.h"
#include "Room.h"
#include "Floor.h"
#include "ModelContainer.h"

using std::make_unique;

Room::Room(QOpenGLWidget* targetWidget, GLfloat roomWidth, glm::vec3 wallColor, glm::vec3 floorColor) :
    width{ roomWidth }, wallColor{ wallColor }, floorColor{ floorColor }
{
    this->targetWidget = targetWidget;
    floor = make_unique<Floor>(targetWidget, width);
    floor->SetColor(this->floorColor);

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

//Overloaded constructor with initial textures
Room::Room(QOpenGLWidget* targetWidget, GLfloat roomWidth, glm::vec3 wallColor, glm::vec3 floorColor, std::string wallTexturePath, std::string floorTexturePath):
	width{ roomWidth }, wallColor{ wallColor }, floorColor{ floorColor }, wallTexture{ wallTexturePath } ,floorTexture{ floorTexturePath }
{
	this->targetWidget = targetWidget;
	floor = make_unique<Floor>(targetWidget, width);
	floor->SetColor(this->floorColor);
	//set floor Texture
	floor->SetTexture(this->floorTexture);

	auto leftWall = make_shared<Wall>(targetWidget, width / 2.0f, width, RIGHT_WALL, WALL_COLOR);
	leftWall->SetColor(this->wallColor);
	leftWall->SetTexture(this->wallTexture);

	auto rightWall = make_shared<Wall>(targetWidget, width / 2.0f, width, LEFT_WALL, WALL_COLOR);
	rightWall->SetColor(this->wallColor);
	rightWall->SetTexture(this->wallTexture);

	auto backWall = make_shared<Wall>(targetWidget, width / 2.0f, width, BACK_WALL, WALL_COLOR);
	backWall->SetColor(this->wallColor);
	backWall->SetTexture(this->wallTexture);



	walls.push_back(leftWall);
	walls.push_back(rightWall);
	walls.push_back(backWall);
}


Room::~Room()
{
}

void Room::SetWallColor(glm::vec3 wallColor)
{
    this->wallColor = wallColor;
    for (auto wall : walls)
        wall->SetColor(wallColor);
}

void Room::SetFloorColor(glm::vec3 floorColor)
{
    this->floorColor = floorColor;
    floor->SetColor(floorColor);
}

void Room::SetWallTexture(string wallTexture)
{
    this->wallTexture = wallTexture;
    for (auto wall : walls)
        wall->SetTexture(this->wallTexture);
}

void Room::SetFloorTexture(string floorTexture)
{
    this->floorTexture = floorTexture;
    floor->SetTexture(this->floorTexture);
}

glm::vec3 Room::GetMinimumVertices() const
{
    return glm::vec3(-width / 2.0f, 0.0f, -width / 2.0f);
}

glm::vec3 Room::GetMaximumVertices() const
{
    return glm::vec3(width / 2.0f, width / 2.0f, width / 2.0f);
}

void Room::Draw(glm::mat4 const& view, glm::mat4 const& projection) const
{
    // Don't write to the stencil buffer
    glStencilMask(0x00);

    for (auto wall : walls)
        wall->Draw(view, projection);

    floor->Draw(view, projection);
}

void Room::BindToFloor(ModelContainer* container)
{
    // Set the rotation bound to be 'y' axis, and the translation bound to be 'xz'
    container->SetRotationBound(glm::vec3(0.0f, 1.0f, 0.0f));
    container->SetTranslationBound(glm::vec3(1.0f, 0.0f, 1.0f));

    // Just a small offset from the ground
    container->SetInitialTranslateVector(glm::vec3(0.0f, 0.001f, 0.0f));
}

void Room::BindToWall(ModelContainer* container, Location wallLocation)
{
    for (auto wall : walls)
        if (wall->GetLocation() == wallLocation)
            wall->Bind(container);
}
