#pragma once

#include "Includes.h"
#include "Model.h"
#include "Room.h"
///
/// \brief The ModelContainer class 
/// the model's container controls the size,scaling, collision, the bounding boxes
///
class ModelContainer : public QObject
{
    Q_OBJECT

    Model* model = nullptr;

    glm::vec3 translateVector = glm::vec3(0.0f);
    glm::vec3 scaleVector = glm::vec3(0.0f);
    glm::vec3 initialScaleVector = glm::vec3(1.0f);
    glm::vec3 initialTranslateVector = glm::vec3(0.0f);
    glm::vec3 rotationAngles = glm::vec3(0.0f);
    glm::vec3 initialRotationAngles = glm::vec3(0.0f);
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
    glm::vec3 scaleBound = glm::vec3(1.0f);
    Room* room = nullptr;

    // Used when a model is bounded to a wall
    Location boundedWall;

    glm::mat4 CombineTransformations() const;
    void UpdateBoundingBox();
    void DetectCollision();

signals:
    // Used to display a message when the transformation is not allowed
    void OperationNotAllowed(QString message);
    void OperationSuccessful();
    
public:
    ///
    /// \brief ModelContainer 
    /// Insert the model into the room provide initial scale factor and target width. 
    /// Check that it fits into the room and that it is currently not colliding with anything.
    /// \param model
    /// \param initialScale
    /// \param room
    /// \param targetWidget
    ///
    ModelContainer(Model* model, GLfloat initialScale, Room* room, QOpenGLWidget* targetWidget);
    ~ModelContainer();

    GLfloat GetScaleFactor() const { return scaleFactor; }

    glm::vec3 GetScaleVector() const { return scaleVector; }
    glm::vec3 GetTranslationVector() const { return translateVector; }
    glm::vec3 GetRotationAngles() const { return rotationAngles; }
    vector<glm::vec3> const& GetBoundingBoxVertices() const { return aaBoundingBoxVertices; }
    glm::vec3 GetMinimumVertices() const { return aaBoundingBoxVertices[0]; }
    glm::vec3 GetMaximumVertices() const { return aaBoundingBoxVertices[6]; }
    glm::vec3 getModelContainerCenter() const { return modelContainerCenter;}
    glm::mat4 GetModelMatrix() const { return CombineTransformations(); }

    // Setters for initial values
    void SetInitialScale(GLfloat initialScale);
    void SetInitialTranslateVector(glm::vec3 initialTranslateVector);
    void SetInitialRotationAngles(glm::vec3 initialRotationAngles);

    // Setter and getter for the bounded wall
    Location GetBoundedWall() const { return boundedWall; }
    void SetBoundedWall(Location wallLocation) { boundedWall = wallLocation; }

    // Getter for the contained model
    Model* GetModel() const { return model; }

    // Get the transformation matrix
    glm::mat4 GetTransformMatrix() const;

    /// Get the transform-rotate matrix
    glm::mat4 GetTransformRotateMatrix() const;

    void SetProjectionMatrix(glm::mat4& projection) { this->projection = projection; }
    void SetViewMatrix(glm::mat4& view) { this->view = view; }
    void SetTranslationBound(glm::vec3 translationBound) { this->translationBound = translationBound; }
    void SetRotationBound(glm::vec3 rotationBound) { this->rotationBound = rotationBound; }
    void SetScaleBound(glm::vec3 scaleBound) { this->scaleBound = scaleBound; }
    ///
    /// \brief ScaleBy
    /// \param scaleFactor
    ///
    void ScaleBy(GLfloat scaleFactor);
    ///
    /// \brief RotateBy
    /// \param angles
    ///
    void RotateBy(glm::vec3 angles);
    ///
    /// \brief TranslateBy
    /// \param translateVector
    ///
    void TranslateBy(glm::vec3 translateVector);
    bool IsSelected() const { return selected; }
    bool SetSelected(bool selected);
    bool IsColliding() const { return colliding; }
    ///
    /// \brief DrawModel draws the model and handles collision drawing
    /// \param shader
    /// \param outlineShader
    /// \param index
    ///
    void DrawModel(Shader& shader, Shader& outlineShader, int index);
    ///
    /// \brief DrawModelBoundingBox
    /// \param shader
    ///
    void DrawModelBoundingBox(Shader& shader);
    ///
    /// \brief DrawAABoudningBox
    /// \param shader
    ///
    void DrawAABoudningBox(Shader& shader);

    void Reset();
    bool IsInsideRoom() const;

private:
    //Container center location in scene. Used for the camera to rotate around this object when selected.
    glm::vec3 modelContainerCenter;
};
