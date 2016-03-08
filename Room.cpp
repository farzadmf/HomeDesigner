#include "Shader.h"
#include "Room.h"
#include "Floor.h"
#include "Wall.h"

using std::make_unique;

Room::Room(QOpenGLWidget* targetWidget, GLfloat roomWidth) : width{ roomWidth }
{
    this->targetWidget = targetWidget;
    floor = make_unique<Floor>(targetWidget, width);
    walls.push_back(make_shared<Wall>(targetWidget, width / 2.0f, width, RIGHT));
    walls.push_back(make_shared<Wall>(targetWidget, width / 2.0f, width, LEFT));
    walls.push_back(make_shared<Wall>(targetWidget, width / 2.0f, width, TOP));
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
