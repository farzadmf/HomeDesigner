#pragma once

#include "Includes.h"
#include <QOpenGLWidget>
#include "Shader.h"

class ModelContainer;
///
/// \brief The Location enum
///
enum Location
{
    LEFT_WALL,
    RIGHT_WALL,
    BACK_WALL,
};
///
/// \brief The WallRenderMode enum
///
enum WallRenderMode
{
    WALL_COLOR,
    WALL_TEXTURE
};
///
/// \brief The Wall class
///
class Wall
{
    GLfloat distance;
    GLfloat width;
    Location location;

    QOpenGLWidget* targetWidget = nullptr;
    GLuint wallVao = 0;
    GLuint wallVbo = 0;
    GLuint wallEbo = 0;
    GLuint secondWallVao = 0;
    GLuint secondWallVbo = 0;
    GLuint secondWallEbo = 0;

    unique_ptr<Shader> shader;

    WallRenderMode renderMode;      // Render with color or texture
    glm::vec3 color;
    GLuint textureId;

    void BufferData() const;

public:
    ///
    /// \brief Wall
    /// \param targetWidget
    /// \param distance
    /// \param width
    /// \param location
    /// \param renderMode
    ///
    Wall(QOpenGLWidget* targetWidget, GLfloat distance, GLfloat width, Location location, WallRenderMode renderMode);
    ~Wall();

    // Getter for location
    Location GetLocation() const { return location; }

    // Convert a string to location enumeration
    static Location GetLocationByName(string locationName);

    // Getter and setter for color
    void SetColor(glm::vec3 color);
    glm::vec3 GetColor() const { return color; }

    // Getter and setter for texture
    void SetTexture(string textureFilePath);
    GLuint GetTextureId() const { return textureId; }
    ///
    /// \brief Draw
    /// \param view
    /// \param projection
    ///
    void Draw(glm::mat4 const& view, glm::mat4 const& projection) const;
    ///
    /// \brief Bind
    /// \param container
    ///
    void Bind(ModelContainer* container) const;

    // Checks whether the specified model container is attached to the wall
    bool IsAttached(ModelContainer* container) const;
};

