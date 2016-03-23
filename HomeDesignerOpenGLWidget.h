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

enum Axis
{
    X,
    Y,
    Z,
    NONE
};

enum Modifiers
{
    SHIFT,
    CONTROL,
    ALT
};

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

public:
    explicit HomeDesignerOpenGLWidget(QWidget *parent = nullptr);
    ~HomeDesignerOpenGLWidget();
    Axis GetAxis() const { return axis; }

    int GetMoveSpeed() const { return moveSpeed; }
    void SetMoveSpeed(int speed) { moveSpeed = speed; }
    int GetRotateSpeed() const { return rotateSpeed; }
    void SetRotateSpeed(int speed) { rotateSpeed = speed; }
    int GetScaleSpeed() const { return scaleSpeed; }
    void SetScaleSpeed(int speed) { scaleSpeed = speed; }

protected:
    void paintGL() override;
    void initializeGL() override;
    void mouseMoveEvent(QMouseEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;

public slots:
    void OnMoveSelectedChanged(bool);
    void OnRotateSelectedChanged(bool);
    void OnScaleSelectedChanged(bool);
    void OnMoveSpeedChanged(int);
    void OnRotateSpeedChanged(int);
    void OnScaleSpeedChanged(int);
    void OnLoadModel(QString modelAttributes, GLfloat initialScale);
    void OnOperationNotAllowed(QString message);
    void OnOperationSuccessful();
    void OnChangeRoomWallColor(QColor color) const;
    void OnChangeRoomWallTexture(QString textureFilePath);
    void OnChangeRoomFloorColor(QColor color) const;
    void OnChangeRoomFloorTexture(QString textureFilePath);

signals:
    void Exit();
    void DisplayMessage(QString message, int timeout);
    void DisplayError(QString message);
    void ClearMessage();
    void AxisChanged(Axis oldAxis, Axis axis);
    void CollisionDetected(bool isColliding);
    void StatusUpdated(bool boundingBox, bool aaBoudningBox, bool axis);
};

