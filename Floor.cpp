#include "Floor.h"
#include "MyHelpers.h"

using std::vector;

void Floor::BufferData() const
{
    vector<glm::vec3> floorVertices =
    {
        glm::vec3(-width / 2.0f, 0.0f, -width / 2.0f),
        glm::vec3(-width / 2.0f, 0.0f, +width / 2.0f),
        glm::vec3(+width / 2.0f, 0.0f, -width / 2.0f),
        glm::vec3(+width / 2.0f, 0.0f, +width / 2.0f)
    };

    vector<GLushort> floorIndices = { 0, 1, 2,  3, 2, 1 };
    vector<GLushort> bottomFloorIndices = { 2, 1, 0,  1, 2, 3 };

    targetWidget->makeCurrent();

    // Main floor
    glBindVertexArray(floorVao);
    glBindBuffer(GL_ARRAY_BUFFER, floorVbo);
    glBufferData(GL_ARRAY_BUFFER, floorVertices.size() * sizeof floorVertices[0], &floorVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, floorIndices.size() * sizeof floorIndices[0], &floorIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &floorVbo);
    glDeleteBuffers(1, &floorEbo);

    // Bottom floor
    glBindVertexArray(bottomFloorVao);
    glBindBuffer(GL_ARRAY_BUFFER, bottomFloorVbo);
    glBufferData(GL_ARRAY_BUFFER, floorVertices.size() * sizeof floorVertices[0], &floorVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottomFloorEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bottomFloorIndices.size() * sizeof bottomFloorIndices[0], &bottomFloorIndices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &bottomFloorVbo);
    glDeleteBuffers(1, &bottomFloorEbo);

    targetWidget->doneCurrent();
}

Floor::Floor(QOpenGLWidget* targetWidget, GLfloat width) : width{ width }
{
    this->targetWidget = targetWidget;
    
    targetWidget->makeCurrent();

    // Create array and buffers
    glGenVertexArrays(1, &floorVao);
    glGenBuffers(1, &floorVbo);
    glGenBuffers(1, &floorEbo);

    glGenVertexArrays(1, &bottomFloorVao);
    glGenBuffers(1, &bottomFloorVbo);
    glGenBuffers(1, &bottomFloorEbo);

    // Create the shader
    shader = make_unique<Shader>("shaders/floor.vs", "shaders/floor.fs");
    shader->Create();

    targetWidget->doneCurrent();

    BufferData();
}

Floor::~Floor()
{
}

void Floor::SetColor(glm::vec3 color)
{
    this->color = color;
    renderMode = FLOOR_COLOR;
}

void Floor::SetTexture(string textureFilePath)
{
    glDeleteTextures(1, &textureId);
    textureId = loadTexture(const_cast<char*>(textureFilePath.c_str()));
    renderMode = FLOOR_TEXTURE;
    cout << "[Floor.cpp]: Now, set the texture for the floor" << endl;
    cout << "textureId = " << textureId << endl;
}

void Floor::Draw(glm::mat4 const& view, glm::mat4 const& projection) const
{
    shader->Use();
    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "model"), 1, GL_FALSE, value_ptr(glm::mat4()));
    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));

    // Set the color/texture
    glUniform1i(glGetUniformLocation(shader->GetProgram(), "textureMode"), renderMode == FLOOR_TEXTURE);
    glUniform3fv(glGetUniformLocation(shader->GetProgram(), "floorColor"), 1, value_ptr(color));
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Main floor
    glBindVertexArray(floorVao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Bottom floor
    glBindVertexArray(bottomFloorVao);
    // This one is always rendered using color
    glUniform1i(glGetUniformLocation(shader->GetProgram(), "textureMode"), false);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}
