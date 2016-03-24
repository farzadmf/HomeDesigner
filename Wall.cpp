#include "Wall.h"
#include "ModelContainer.h"
#include "MyHelpers.h"

void Wall::BufferData() const
{
    vector<GLfloat> wallVertices;

    switch (location)
    {
        case LEFT_WALL:
            wallVertices =
            {
                -distance, +0.0f        , -width / 2.0f, 1.0f, 0.0f,
                -distance, +width / 2.0f, -width / 2.0f, 1.0f, 1.0f,
                -distance, +width / 2.0f, +width / 2.0f, 0.0f, 1.0f,
                -distance, +0.0f        , +width / 2.0f, 0.0f, 0.0f
            };
            break;

        case RIGHT_WALL:
            wallVertices =
            {
                +distance, +0.0f        , +width / 2.0f, 1.0f, 0.0f,
                +distance, +width / 2.0f, +width / 2.0f, 1.0f, 1.0f,
                +distance, +width / 2.0f, -width / 2.0f, 0.0f, 1.0f,
                +distance, +0.0f        , -width / 2.0f, 0.0f, 0.0f
            };
            break;

        case BACK_WALL:
            wallVertices =
            {
                +width / 2.0f, +0.0f        , -distance, 1.0f, 0.0f,
                +width / 2.0f, +width / 2.0f, -distance, 1.0f, 1.0f,
                -width / 2.0f, +width / 2.0f, -distance, 0.0f, 1.0f,
                -width / 2.0f, +0.0f        , -distance, 0.0f, 0.0f
            };
            break;
    }

    vector<GLushort> wallIndices       = {0, 1, 2,  3, 0, 2};
    vector<GLushort> secondWallIndices = {2, 1, 0,  2, 0, 3};

    targetWidget->makeCurrent();

    // Main wall
    glBindVertexArray(wallVao);
    glBindBuffer(GL_ARRAY_BUFFER, wallVbo);
    glBufferData(GL_ARRAY_BUFFER, wallVertices.size() * sizeof wallVertices[0], &wallVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wallIndices.size() * sizeof wallIndices[0], &wallIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof GLfloat, reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof GLfloat, reinterpret_cast<GLvoid*>(3 * sizeof GLfloat));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &wallVbo);
    glDrawBuffers(1, &wallEbo);

    // Second wall
    glBindVertexArray(secondWallVao);
    glBindBuffer(GL_ARRAY_BUFFER, secondWallVbo);
    glBufferData(GL_ARRAY_BUFFER, wallVertices.size() * sizeof wallVertices[0], &wallVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, secondWallEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, secondWallIndices.size() * sizeof secondWallIndices[0], &secondWallIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof GLfloat, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &secondWallVbo);
    glDrawBuffers(1, &secondWallEbo);

    targetWidget->doneCurrent();
}

Wall::Wall(QOpenGLWidget* targetWidget, GLfloat distance, GLfloat width, Location location, WallRenderMode renderMode) :
    distance{ distance }, width{ width }, location{ location }, renderMode{ renderMode }
{
    this->targetWidget = targetWidget;

    targetWidget->makeCurrent();

    // Generate vertex array and buffers
    glGenVertexArrays(1, &wallVao);
    glGenBuffers(1, &wallVbo);
    glGenBuffers(1, &wallEbo);

    glGenVertexArrays(1, &secondWallVao);
    glGenBuffers(1, &secondWallVbo);
    glGenBuffers(1, &secondWallEbo);

    // Create the shader
    shader = make_unique<Shader>("shaders/wall.vs", "shaders/wall.fs");
    shader->Create();

    targetWidget->doneCurrent();

    BufferData();
}

Wall::~Wall()
{
}

Location Wall::GetLocationByName(string locationName)
{
    auto name = QString::fromStdString(locationName);
    if (name.toLower() == "right")
        return RIGHT_WALL;
    if (name.toLower() == "back")
        return BACK_WALL;

    // LEFT_WALL for unknown string!
    return LEFT_WALL;
}

void Wall::SetColor(glm::vec3 color)
{
    this->color = color;
    renderMode = WALL_COLOR;
}

void Wall::SetTexture(string textureFilePath)
{
    glDeleteTextures(1, &textureId);
    textureId = loadTexture(const_cast<char*>(textureFilePath.c_str()));
    renderMode = WALL_TEXTURE;
}

void Wall::Draw(glm::mat4 const& view, glm::mat4 const& projection) const
{
    shader->Use();
    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "model"), 1, GL_FALSE, value_ptr(glm::mat4()));
    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));

    // Set the color/texture
    glUniform1i(glGetUniformLocation(shader->GetProgram(), "textureMode"), renderMode == WALL_TEXTURE);
    glUniform3fv(glGetUniformLocation(shader->GetProgram(), "wallColor"), 1, value_ptr(color));
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Main wall
    glBindVertexArray(wallVao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Second (outer) wall
    shader->Use();
    glBindVertexArray(secondWallVao);
    // This one is always rendered using color
    glUniform1i(glGetUniformLocation(shader->GetProgram(), "textureMode"), false);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}

void Wall::Bind(ModelContainer* container) const
{
    float containerWidth, containerDepth, containerHeight;

    switch (location)
    {
        case LEFT_WALL:
            // Also rotate the model towards the wall (we need to rotate first in order to get
            //      the width and height correctly)
            container->SetInitialRotationAngles(glm::vec3(0.0f, 90.0f, 0.0f));

            // Move the container to wall location (with a small offset); we also need to consider the width of the model
            //      We also move the model to the middle of the wall
            containerWidth = (container->GetMaximumVertices() - container->GetMinimumVertices()).x;
            containerHeight = (container->GetMaximumVertices() - container->GetMinimumVertices()).y;
            container->SetInitialTranslateVector(glm::vec3(-distance + containerWidth / 2.0f + 0.001f,
                                                           distance / 2.0f - containerHeight / 2.0f, 0.0f));

            // Rotation is bound to 'x' and translation to 'yz'
            container->SetRotationBound(glm::vec3(1.0f, 0.0f, 0.0f));
            container->SetTranslationBound(glm::vec3(0.0f, 1.0f, 1.0f));
            container->SetBoundedWall(location);
            container->Reset();

            break;

        case RIGHT_WALL:
            // Also rotate the model towards the wall (we need to rotate first in order to get
            //      the width and height correctly)
            container->SetInitialRotationAngles(glm::vec3(0.0f, -90.0f, 0.0f));

            // Move the container to wall location (with a small offset); we also need to consider the width of the model
            //      We also move the model to the middle of the wall
            containerWidth = (container->GetMaximumVertices() - container->GetMinimumVertices()).x;
            containerHeight = (container->GetMaximumVertices() - container->GetMinimumVertices()).y;
            container->SetInitialTranslateVector(glm::vec3(+distance - containerWidth / 2.0f - 0.001f,
                                                           distance / 2.0f - containerHeight / 2.0f, 0.0f));

            // Rotation is bound to 'x' and translation to 'yz'
            container->SetRotationBound(glm::vec3(1.0f, 0.0f, 0.0f));
            container->SetTranslationBound(glm::vec3(0.0f, 1.0f, 1.0f));
            container->SetBoundedWall(location);
            container->Reset();

            break;

        case BACK_WALL:
            // Move the container to wall location (with a small offset); we also need to consider the depth of the model
            //      We also move the model to the middle of the wall
            containerDepth = (container->GetMaximumVertices() - container->GetMinimumVertices()).z;
            containerHeight = (container->GetMaximumVertices() - container->GetMinimumVertices()).y;
            container->SetInitialTranslateVector(glm::vec3(0.0f, distance / 2.0f - containerHeight / 2.0f,
                                                           -distance + containerDepth / 2.0f + 0.001f));
            // Also rotate the model towards the wall
            container->SetInitialRotationAngles(glm::vec3(0.0f, 0.0f, 0.0f));

            // Rotation is bound to 'z' and translation to 'xy'
            container->SetRotationBound(glm::vec3(0.0f, 0.0f, 1.0f));
            container->SetTranslationBound(glm::vec3(1.0f, 1.0f, 0.0f));
            container->SetBoundedWall(location);
            container->Reset();

            break;
    }
}
