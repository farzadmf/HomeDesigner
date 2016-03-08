#pragma once

#include "Includes.h"
#include "Model.h"

class Room;

enum Type
{
    BOUNDED,
    FREE
};

enum BoundedPlane
{
    XY,
    YZ,
    XZ
};

class ModelContainer : public QObject
{
    Q_OBJECT

    Type type;
    Model* model = nullptr;
    glm::vec3 translateVector = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotationAngles = glm::vec3(0.0f);
    GLfloat scaleFactor = 1.0f;
    GLfloat initialScale = 1.0f;
    bool selected = false;
    bool selectable = true;
    glm::mat4 projection = glm::mat4();
    glm::mat4 view = glm::mat4();
    vector<glm::vec3> aaBoundingBoxVertices;
    int collisionManagerIndex = -1;
    bool colliding = false;
    QOpenGLWidget* targetWidget;
    glm::vec3 translationBound = glm::vec3(1.0f);
    glm::vec3 rotationBound = glm::vec3(1.0f);
    Room* room = nullptr;

    glm::mat4 CombineTransformations() const;
    void UpdateBoundingBox();
    
public:
    ModelContainer(Model* model, GLfloat initialScale, Room* room, QOpenGLWidget* targetWidget);

    GLfloat GetScaleFactor() const { return scaleFactor; }
    glm::vec3 GetTranslationVector() const { return translateVector; }
    glm::vec3 GetRotationAngles() const { return rotationAngles; }
    vector<glm::vec3> const& GetBoundingBoxVertices() const { return aaBoundingBoxVertices; }
    glm::vec3 GetMinimumVertices() const { return aaBoundingBoxVertices[0]; }
    glm::vec3 GetMaximumVertices() const { return aaBoundingBoxVertices[6]; }
    glm::mat4 GetModelMatrix() const { return CombineTransformations(); }

    void SetProjectionMatrix(glm::mat4& projection) { this->projection = projection; }
    void SetViewMatrix(glm::mat4& view) { this->view = view; }
    void SetTranslationBound(glm::vec3 translationBound) { this->translationBound = translationBound; }
    void SetRotationBound(glm::vec3 rotationBound) { this->rotationBound = rotationBound; }

    void ScaleBy(GLfloat scaleFactor);
    void RotateBy(glm::vec3 angles);
    void TranslateBy(glm::vec3 translateVector);
    bool IsSelected() const { return selected; }
    bool SetSelected(bool selected);
    bool IsColliding() const { return colliding; }

    void DrawModel(Shader& shader, Shader& outlineShader);
    void DrawModelBoundingBox(Shader& shader);
    void DrawAABoudningBox(Shader& shader);

    void Reset();
    bool IsInsideRoom() const;
};