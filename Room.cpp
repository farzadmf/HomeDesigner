#include "Shader.h"
#include "Room.h"
#include "Floor.h"
#include "ModelContainer.h"

using std::make_unique;

Room::Room(QOpenGLWidget* targetWidget, GLfloat roomWidth) : width{ roomWidth }
{
    this->targetWidget = targetWidget;
    floor = make_unique<Floor>(targetWidget, width);
    walls.push_back(make_shared<Wall>(targetWidget, width / 2.0f, width, RIGHT_WALL));
    walls.push_back(make_shared<Wall>(targetWidget, width / 2.0f, width, LEFT_WALL));
    walls.push_back(make_shared<Wall>(targetWidget, width / 2.0f, width, BACK_WALL));
}

Room::~Room()
{
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
