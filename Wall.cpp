#include "Wall.h"
#include "ModelContainer.h"

void Wall::BufferData() const
{
    vector<glm::vec3> wallVertices;

    switch (location)
    {
        case LEFT_WALL:
            wallVertices.push_back(glm::vec3(-distance, +0.0f        , -width / 2.0f));
            wallVertices.push_back(glm::vec3(-distance, +width / 2.0f, -width / 2.0f));
            wallVertices.push_back(glm::vec3(-distance, +width / 2.0f, +width / 2.0f));
            wallVertices.push_back(glm::vec3(-distance, +0.0f        , +width / 2.0f));
            break;
        case RIGHT_WALL:
            wallVertices.push_back(glm::vec3(+distance, +0.0f        , +width / 2.0f));
            wallVertices.push_back(glm::vec3(+distance, +width / 2.0f, +width / 2.0f));
            wallVertices.push_back(glm::vec3(+distance, +width / 2.0f, -width / 2.0f));
            wallVertices.push_back(glm::vec3(+distance, +0.0f        , -width / 2.0f));
            break;
        case BACK_WALL:
            wallVertices.push_back(glm::vec3(+width / 2.0f, +0.0f        , -distance));
            wallVertices.push_back(glm::vec3(+width / 2.0f, +width / 2.0f, -distance));
            wallVertices.push_back(glm::vec3(-width / 2.0f, +width / 2.0f, -distance));
            wallVertices.push_back(glm::vec3(-width / 2.0f, +0.0f        , -distance));
            break;
    }

    vector<GLushort> wallIndices = {0, 1, 2,  3, 0, 2};
    vector<GLushort> secondWallIndices = {2, 1, 0,  2, 0, 3};

    targetWidget->makeCurrent();

    // Main wall
    glBindVertexArray(wallVao);
    glBindBuffer(GL_ARRAY_BUFFER, wallVbo);
    glBufferData(GL_ARRAY_BUFFER, wallVertices.size() * sizeof wallVertices[0], &wallVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wallIndices.size() * sizeof wallIndices[0], &wallIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(0));
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &secondWallVbo);
    glDrawBuffers(1, &secondWallEbo);

    targetWidget->doneCurrent();
}

Wall::Wall(QOpenGLWidget* targetWidget, GLfloat distance, GLfloat width, Location location) :
    distance{ distance }, width{ width }, location { location }
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

void Wall::Draw(glm::mat4 const& view, glm::mat4 const& projection) const
{
    shader->Use();
    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "model"), 1, GL_FALSE, value_ptr(glm::mat4()));
    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));
    
    // Main wall
    glBindVertexArray(wallVao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);

    // Second wall
    shader->Use();
    glBindVertexArray(secondWallVao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}

void Wall::Bind(ModelContainer* container) const
{
    switch (location)
    {
        case LEFT_WALL:
//            container->SetTranslationBound()
            break;
        case RIGHT_WALL:
            break;
        case BACK_WALL:
            break;
    }
}
