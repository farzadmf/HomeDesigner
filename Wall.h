#pragma once

#include "Includes.h"
#include <QOpenGLWidget>
#include "Shader.h"

class ModelContainer;
///
/// \brief The Location enum determines which wall
///
enum Location
{
    LEFT_WALL,
    RIGHT_WALL,
    BACK_WALL,
};
///
/// \brief The WallRenderMode enum either color or texture
///
enum WallRenderMode
{
    WALL_COLOR,
    WALL_TEXTURE
};
///
/// \brief The Wall class 
///  Renders and keeps track of the wall,colour,texture and size
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
    /// \brief Wall renders the wall based on the distance ,location , width ,wall location and based on color or texture
    /// \param targetWidget
    /// \param distance the distance
    /// \param width the width
    /// \param location which wall
    /// \param renderMode the color of the texture
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
    /// \brief Draw draws the colors textures bind the main and the other walls
    /// \param view
    /// \param projection
    ///
    void Draw(glm::mat4 const& view, glm::mat4 const& projection) const;
    ///
    /// \brief Bind binds the model to the wall
    /// \param container
    ///
    void Bind(ModelContainer* container) const;

    // Checks whether the specified model container is attached to the wall
    bool IsAttached(ModelContainer* container) const;
};

