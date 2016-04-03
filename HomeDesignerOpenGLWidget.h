#pragma once

#include "Includes.h"
#include "Shader.h"
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <memory>
#include "Model.h"
#include "Camera.h"
#include "ModelContainer.h"
#include "Room.h"
///
/// \brief The x,y,z Axis enum
///
enum Axis
{
    X,
    Y,
    Z,
    NONE
};
///
/// \brief The Modifiers key enum
///
enum Modifiers
{
    SHIFT,
    CONTROL,
    ALT
};
///
/// \brief The HomeDesignerOpenGLWidget class Which creates the view and projection matrices using perspective viewing,processes keyboard and mouse input allowing movement and zooming,Iterate through all models in the scene, draw them as necessary (including the bounding boxes and outline),collision detection 
///
class HomeDesignerOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

    shared_ptr<Room> room;
    QElapsedTimer timer;
    GLfloat angleStep = 1.0f;
    GLfloat moveStep = 0.01f;
    GLfloat scaleStep = 0.002f;
    int selectedContainerIndex = -1;
    unique_ptr<Shader> shader;
    shared_ptr<Shader> outlineShader;
    unique_ptr<Model> nanosuitModel;
    map<string, unique_ptr<Model>> models;
    vector<unique_ptr<ModelContainer>> modelContainers;
    unique_ptr<Camera> camera;
    bool leftMouseButtonDown = false;
    bool rightMouseButtonDown = false;
    bool middleMouseButtonDown = false;
    bool isMove = false;
    bool isRotate = false;
    bool isScale = false;
    int moveSpeed = 1;
    int rotateSpeed = 1;
    int scaleSpeed = 1;
    Axis axis = NONE;
    bool collisionDetected = false;

    // Last message from this class being displayed in the status bar
    QString lastMessage;    

    void ProcessKeyboard();
    void EmitDisplayMessage(QString message, int timeout);
    void EmitClearMessage();

public:
    ///
    /// \brief HomeDesignerOpenGLWidget
    /// \param parent
    ///
    explicit HomeDesignerOpenGLWidget(QWidget *parent = nullptr);
    ~HomeDesignerOpenGLWidget();
    ///
    /// \brief GetAxis
    /// \return
    ///
    Axis GetAxis() const { return axis; }
    ///
    /// \brief GetMoveSpeed
    /// \return
    ///
    int GetMoveSpeed() const { return moveSpeed; }
    ///
    /// \brief SetMoveSpeed
    /// \param speed
    ///
    void SetMoveSpeed(int speed) { moveSpeed = speed; }
    ///
    /// \brief GetRotateSpeed
    /// \return
    ///
    int GetRotateSpeed() const { return rotateSpeed; }
    ///
    /// \brief SetRotateSpeed
    /// \param speed
    ///
    void SetRotateSpeed(int speed) { rotateSpeed = speed; }
    ///
    /// \brief GetScaleSpeed
    /// \return
    ///
    int GetScaleSpeed() const { return scaleSpeed; }
    ///
    /// \brief SetScaleSpeed
    /// \param speed
    ///
    void SetScaleSpeed(int speed) { scaleSpeed = speed; }

protected:
    ///
    /// \brief paintGL Main loop of the application
    ///
    void paintGL() override;
    ///
    /// \brief initializeGL Called when the widget is being initialized
    ///
    void initializeGL() override;
    ///
    /// \brief mouseMoveEvent Process mouse movement
    ///
    void mouseMoveEvent(QMouseEvent*) override;
    ///
    /// \brief keyPressEvent Process key press event
    ///
    void keyPressEvent(QKeyEvent*) override;
    ///
    /// \brief keyReleaseEvent Reset pressed keys and modifiers, along with status bar message
    ///
    void keyReleaseEvent(QKeyEvent*) override;
    ///
    /// \brief mousePressEvent Keep track of mouse button(s) being pressed
    ///
    void mousePressEvent(QMouseEvent*) override;
    ///
    /// \brief mouseReleaseEvent Reset mouse buttons and release mouse
    ///
    void mouseReleaseEvent(QMouseEvent*) override;
    ///
    /// \brief wheelEvent Allow Zoom in/out with mouse wheel
    ///
    void wheelEvent(QWheelEvent*) override;

public slots:
    ///
    /// \brief OnMoveSelectedChanged called when move selection changed
    ///
    void OnMoveSelectedChanged(bool);
    ///
    /// \brief OnRotateSelectedChanged called when rotation selection changed
    ///
    void OnRotateSelectedChanged(bool);
    ///
    /// \brief OnScaleSelectedChanged called when scale selection changed
    ///
    void OnScaleSelectedChanged(bool);
    ///
    /// \brief OnMoveSpeedChanged called when the move speed changed
    ///
    void OnMoveSpeedChanged(int);
    ///
    /// \brief OnRotateSpeedChanged calledon rotation speed changed
    ///
    void OnRotateSpeedChanged(int);
    ///
    /// \brief OnScaleSpeedChanged called on scale speed changed
    ///
    void OnScaleSpeedChanged(int);
    ///
    /// \brief OnLoadModel  Loads a model from the file, setting its initial scale value
    /// \param modelAttributes
    /// \param initialScale
    ///
    void OnLoadModel(QString modelAttributes, GLfloat initialScale);
    ///
    /// \brief OnOperationNotAllowed  Displays a message when the operation (move, rotate, scale) isn't allowed
    /// \param message the message
    ///
    void OnOperationNotAllowed(QString message);
    ///
    /// \brief OnOperationSuccessful  Restores the previous message in the status bar when the operation is successful
    ///
    void OnOperationSuccessful();
    ///
    /// \brief OnChangeRoomWallColor Changes the wall color for the room
    /// \param color
    ///
    void OnChangeRoomWallColor(QColor color) const;
    ///
    /// \brief OnChangeRoomWallTexture Changes the wall texture
    /// \param textureFilePath
    ///
    void OnChangeRoomWallTexture(QString textureFilePath) const;
    ///
    /// \brief OnChangeRoomFloorColor changes the floor color
    /// \param color
    ///
    void OnChangeRoomFloorColor(QColor color) const;
    ///
    /// \brief OnChangeRoomFloorTexture changes the room texture
    /// \param textureFilePath
    ///
    void OnChangeRoomFloorTexture(QString textureFilePath) const;

signals:
    void Exit();
    void DisplayMessage(QString message, int timeout);
    void DisplayError(QString message);
    void ClearMessage();
    void AxisChanged(Axis oldAxis, Axis axis);
    void CollisionDetected(bool isColliding);
    void UpdateStatus(bool boundingBox, bool aaBoudningBox, bool worldAxis, bool localAxis);
};

