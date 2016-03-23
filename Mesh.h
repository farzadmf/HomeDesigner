#pragma once

#include "Includes.h"
#include "Shader.h"
#include <QOpenGLWidget>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoordinates;
};

struct Texture
{
    GLuint Id;
    string Type;
    aiString Path;
};

class Mesh
{
public:
    // Mesh data
    vector<Vertex> Vertices;
    vector<GLuint> Indices;
    vector<Texture> Textures;

    // Functions
    Mesh(QOpenGLWidget* targetWidget, vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures) :
        Vertices{ vertices }, Indices{ indices }, Textures{ textures }, targetWidget{ targetWidget }
    {
        SetupMesh();
    }

    GLuint GetVao() const { return vao; }

    void Draw(Shader shader)
    {
        GLuint diffuseNumber = 1;
        GLuint specularNumber = 1;

        for (GLuint i = 0; i < Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            // Retrieve texture number (the N in 'diffuse_textureN')
            string number;
            string name = Textures[i].Type;

            if (name == "texture_diffuse")
                number = std::to_string(diffuseNumber++);
            else if (name == "texture_specular")
                number = std::to_string(specularNumber++);

//            glUniform1i(glGetUniformLocation(shader.GetProgram(), ("material." + name + number).c_str()), i);
            glUniform1i(glGetUniformLocation(shader.GetProgram(), (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, Textures[i].Id);
        }

        glUniform1f(glGetUniformLocation(shader.GetProgram(), "material.shininess"), 16.0f);

        // Draw mesh
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // Good practice to set everything back to defaults once configured
        for (GLuint i = 0; i < Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

private:
    GLuint vao, vbo, ebo;
    QOpenGLWidget* targetWidget;

    void SetupMesh()
    {
        targetWidget->makeCurrent();
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), &Indices[0], GL_STATIC_DRAW);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
        // Vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, Normal)));
        // Vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, TextureCoordinates)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        targetWidget->doneCurrent();
    }
};

