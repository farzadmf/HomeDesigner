#include "HomeDesignerOpenGLWidget.h"
#include <QMouseEvent>
#include <QCoreApplication>

using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;

static int lastMouseX;
static int lastMouseY;
static map<int, bool> keys;
static bool modifiers[3];
static GLfloat cameraMoveSpeed = 0.2f;
static bool firstMouseMovement = true;

static bool drawBoundingBox = false;
static bool drawAABoundingBox = false;
static bool showLocalAxis = false;
static bool showWorldAxis = false;

static GLuint axisVao, axisVbo, axisEbo;
static Shader axisShader;

static glm::vec3 initialCameraPosition(0.0f, 50.0f, 80.0f);
static glm::vec2 initialCameraDelta(0.0f, -100.0f);
static GLfloat roomWidth = 60.0f;

HomeDesignerOpenGLWidget::HomeDesignerOpenGLWidget(QWidget* parent) :
    QOpenGLWidget(parent)
{
    timer.start();
}

HomeDesignerOpenGLWidget::~HomeDesignerOpenGLWidget()
{
}

void HomeDesignerOpenGLWidget::paintGL()
{
    // We capture keyboard state in 'keyboardEvent' and call this function to process the keys.
    ProcessKeyboard();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom),
                                            static_cast<GLfloat>(width()) / static_cast<GLfloat>(height()), 0.1f, 1000.0f);

    axisShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(axisShader.GetProgram(), "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(axisShader.GetProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));

    if (showWorldAxis)
    {
        GLfloat currentLineWidth;
        glGetFloatv(GL_LINE_WIDTH, &currentLineWidth);
        glLineWidth(2 * currentLineWidth);
        glUniformMatrix4fv(glGetUniformLocation(axisShader.GetProgram(), "model"), 1, GL_FALSE, value_ptr(glm::mat4()));
        glBindVertexArray(axisVao);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);
        glLineWidth(currentLineWidth);
    }

    room->Draw(view, projection);

    // Iterate through all models in the scene, draw them as necessary (including the bounding boxes and outline)
    //      and also see if there's any collisions
    collisionDetected = false;
    for (auto& container : modelContainers)
    {
        container->SetProjectionMatrix(projection);
        container->SetViewMatrix(view);
        container->DrawModel(*shader, *outlineShader);
        if (drawBoundingBox)
            container->DrawModelBoundingBox(*outlineShader);
        if (drawAABoundingBox)
            container->DrawAABoudningBox(*outlineShader);

        if (container->IsColliding())
            collisionDetected = true;

        if (container->IsSelected() && showLocalAxis)
        {
            axisShader.Use();
            glUniformMatrix4fv(glGetUniformLocation(axisShader.GetProgram(), "model"), 1, GL_FALSE,
                               value_ptr(container->GetModelMatrix()));
            glBindVertexArray(axisVao);
            glDrawArrays(GL_LINES, 0, 6);
            glBindVertexArray(0);
        }
    }

    // If no collision is detected, we want to "enable" the button, so we should pass "true"
    emit CollisionDetected(!collisionDetected);
}

void HomeDesignerOpenGLWidget::initializeGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    // Debugging information
//    const GLubyte* renderer = glGetString(GL_RENDERER);
//    const GLubyte* version = glGetString(GL_VERSION);
//    cout << endl << "++++++++++++++++++++++++++++ OpenGL Info ++++++++++++++++++++++++++++" << endl;
//    cout << setw(40) << left << "Renderer:" << renderer << endl;
//    cout << setw(40) << left << "Maximum OpenGL Version Supported:" << version << endl;
//    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    axisShader = Shader("shaders/axis.vs", "shaders/axis.fs");
    axisShader.Create();
    GLfloat axes[] =
    {
        // x axis (cyan)
        0.0f, 0.0005f, 0.0f, 0.5f, 0.0f, 0.0f,
        8.0f, 0.0005f, 0.0f, 0.5f, 0.0f, 0.0f,

        // y axis (green)
        0.0f, 0.0005f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 8.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        // z axis (blue)
        0.0f, 0.0005f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0005f, 8.0f, 0.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &axisVao);
    glGenBuffers(1, &axisVbo);
    glGenBuffers(1, &axisEbo);
    glBindVertexArray(axisVao);
    glBindBuffer(GL_ARRAY_BUFFER, axisVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof axes, axes, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GLfloat, reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GLfloat, reinterpret_cast<GLvoid*>(3 * sizeof GLfloat));
    glBindVertexArray(0);

    shader = make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
    shader->Create();
    outlineShader = make_shared<Shader>("shaders/outline.vs", "shaders/outline.fs");
    outlineShader->Create();
    camera = make_unique<Camera>(initialCameraPosition);
    camera->ProcessMouseMovement(initialCameraDelta.x, initialCameraDelta.y);
    camera->MouseSensitivity = 0.5f;
    camera->MovementSpeed = 5.0f;

    room = make_shared<Room>(this, roomWidth);
}

void HomeDesignerOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Move the camera if right mouse button is down and scene isn't empty
    if (rightMouseButtonDown && modelContainers.size() > 0)
    {
        if (firstMouseMovement)
        {
            lastMouseX = event->x();
            lastMouseY = event->y();
            firstMouseMovement = false;
        }

        int xOffset = lastMouseX - event->x();
        int yOffset = event->y() - lastMouseY;
        lastMouseX = event->x();
        lastMouseY = event->y();
        camera->ProcessMouseMovement(xOffset, yOffset);
        update();
    }

    if (!leftMouseButtonDown || selectedContainerIndex == -1)
        return;

    ModelContainer* selectedContainer = modelContainers[selectedContainerIndex].get();

    if (isMove && axis != NONE)
    {
        GLfloat translateFactor = moveSpeed * (event->y() < lastMouseY ? moveStep : -moveStep);
        lastMouseY = event->y();
        glm::vec3 targetTranslate;
        switch (axis)
        {
            case X:
                targetTranslate = glm::vec3(translateFactor, 0.0f, 0.0f);
                break;
            case Y:
                targetTranslate = glm::vec3(0.0f, translateFactor, 0.0f);
                break;
            default:
                targetTranslate = glm::vec3(0.0f, 0.0f, translateFactor);
                break;
        }
        selectedContainer->TranslateBy(targetTranslate);
        update();
    }

    if (isScale && axis == NONE)
    {
        GLfloat scaleFactor = scaleSpeed * (event->y() < lastMouseY ? scaleStep : -scaleStep);
        lastMouseY = event->y();
        GLfloat currentScaleFactor = selectedContainer->GetScaleFactor();
        GLfloat targetScaleFactor = currentScaleFactor + scaleFactor > 0 ? currentScaleFactor + scaleFactor : 0.01f;
        selectedContainer->ScaleBy(targetScaleFactor);
        update();
    }

    if (isRotate && axis != NONE)
    {
        GLfloat angle = rotateSpeed * (event->y() < lastMouseY ? angleStep : -angleStep);
        lastMouseY = event->y();
        glm::vec3 targetRotationAngles;
        switch (axis)
        {
            case X:
                targetRotationAngles = glm::vec3(angle, 0.0f, 0.0f);
                break;
            case Y:
                targetRotationAngles = glm::vec3(0.0f, angle, 0.0f);
                break;
            default:
                targetRotationAngles = glm::vec3(0.0f, 0.0f, angle);
                break;
        }
        selectedContainer->RotateBy(targetRotationAngles);
        update();
    }
}

void HomeDesignerOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    keys[event->key()] = true;
    modifiers[SHIFT] = event->modifiers() & Qt::ShiftModifier;
    modifiers[CONTROL] = event->modifiers() & Qt::ControlModifier;
    modifiers[ALT] = event->modifiers() & Qt::AltModifier;

    Axis oldAxis = axis;
    switch (event->key())
    {
        case Qt::Key_X:
            axis = X;
            if (oldAxis != axis)
            {
                lastMessage = "<b><font color='maroon'>Axis   -------->   x  ( MAROON )</font></b>";
                emit DisplayMessage(lastMessage, 0);
            }
            break;

        case Qt::Key_Y:
            axis = Y;
            if (oldAxis != axis)
            {
                lastMessage = "<b><font color='green'>Axis   -------->   y  ( GREEN )</font></b>";
                emit DisplayMessage(lastMessage, 0);
            }
            break;

        case Qt::Key_Z:
            axis = Z;
            if (oldAxis != axis)
            {
                lastMessage = "<b><font color='blue'>Axis   -------->   z  ( BLUE )</font></b>";
                emit DisplayMessage(lastMessage, 0);
            }
            break;

        default:
            axis = NONE;
            lastMessage = "";
            emit ClearMessage();
            break;
    }

    if (event->key() == Qt::Key_Q)
        emit Exit();

    update();
}

void HomeDesignerOpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        return;

    axis = NONE;
    lastMessage = "";
    emit ClearMessage();
    keys[event->key()] = false;
    modifiers[SHIFT] = event->modifiers() & Qt::ShiftModifier;
    modifiers[CONTROL] = event->modifiers() & Qt::ControlModifier;
    modifiers[ALT] = event->modifiers() & Qt::AltModifier;
}

void HomeDesignerOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    leftMouseButtonDown = event->button() == Qt::LeftButton;
    rightMouseButtonDown = event->button() == Qt::RightButton;
    if (rightMouseButtonDown)
        grabMouse(Qt::ClosedHandCursor);
}

void HomeDesignerOpenGLWidget::mouseReleaseEvent(QMouseEvent*)
{
    leftMouseButtonDown = rightMouseButtonDown = false;
    firstMouseMovement = true;
    releaseMouse();
}

void HomeDesignerOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    camera->ProcessMouseScroll(event->delta() > 0 ? 1 : -1);
    update();
}

void HomeDesignerOpenGLWidget::OnMoveSelectedChanged(bool selected) { isMove = selected; }
void HomeDesignerOpenGLWidget::OnRotateSelectedChanged(bool selected) { isRotate = selected; }
void HomeDesignerOpenGLWidget::OnScaleSelectedChanged(bool selected) { isScale = selected; }
void HomeDesignerOpenGLWidget::OnMoveSpeedChanged(int speed) { moveSpeed = speed; }
void HomeDesignerOpenGLWidget::OnRotateSpeedChanged(int speed) { rotateSpeed = speed; }
void HomeDesignerOpenGLWidget::OnScaleSpeedChanged(int speed) { scaleSpeed = speed; }

void HomeDesignerOpenGLWidget::OnLoadModel(int modelIndex, QString modelAttributes, GLfloat initialScale)
{
    auto attributes = modelAttributes.split('|');
    auto modelPath = attributes[0].toStdString();

    // Check if the map already contains a key for this model
    bool modelAlreadyLoaded = false;
    for (auto& model : models)
    {
        if (model.first == modelPath)
        {
            modelAlreadyLoaded = true;
            break;
        }
    }

    // Only load the model if it's not already loaded
    if (!modelAlreadyLoaded)
    {
        string message = "Loading Model in path: '" + modelPath + "' ...... ";
        emit DisplayMessage(QString::fromStdString(message), 0);
        QCoreApplication::processEvents();
        models[modelPath] = make_unique<Model>(modelPath, this);
        message += "DONE!";
        emit DisplayMessage(QString::fromStdString(message), 3000);
    }

    auto myRoom = room.get();
    auto myModel = models[modelPath].get();
    auto container = make_unique<ModelContainer>(myModel, initialScale, myRoom, this);
//    auto container = make_unique<ModelContainer>(models[modelPath].get(), initialScale, room.get(), this);
    if (!container->IsInsideRoom())
    {
        emit DisplayError("The model object is too big! Please try again.");
        container.release();
        return;
    }

    container->SetSelected(true);

    // Attach container's SIGNAL to our SLOT to display a message
    connect(container.get(), SIGNAL(OperationNotAllowed(QString)), this, SLOT(OnOperationNotAllowed(QString)));
    connect(container.get(), SIGNAL(OperationSuccessful()), this, SLOT(OnOperationSuccessful()));

    // The model is to be bound to somewhere
    if (attributes.size() > 1)
    {
        if (attributes[1].toLower() == "floor")
            room->BindToFloor(container.get());
        else if (attributes[1].toLower() == "wall")
        {
            auto wallLocation = Wall::GetLocationByName(attributes[2].toStdString());
            room->BindToWall(container.get(), wallLocation);
        }
    }

    for (int i = 0; i < modelContainers.size(); i++)
        modelContainers[i]->SetSelected(false);

    modelContainers.push_back(std::move(container));
    selectedContainerIndex = modelContainers.size() - 1;
    grabKeyboard();
    update();
}

void HomeDesignerOpenGLWidget::OnOperationNotAllowed(QString message)
{
    emit DisplayMessage(message, 0);
}

void HomeDesignerOpenGLWidget::OnOperationSuccessful()
{
    emit DisplayMessage(lastMessage, 0);
}

void HomeDesignerOpenGLWidget::ProcessKeyboard()
{
    if (keys[Qt::Key_Space])
    {
        if (collisionDetected || modelContainers.size() == 0)
            return;

        while (true)
        {
            if (selectedContainerIndex != -1)
                modelContainers[selectedContainerIndex]->SetSelected(false);

            selectedContainerIndex = (selectedContainerIndex + 1) % modelContainers.size();
            if (modelContainers[selectedContainerIndex]->SetSelected(true))
                break;
        }
    }

    if (keys[Qt::Key_Escape])
    {
        if (collisionDetected)
            return;

        for (auto i = 0; i < modelContainers.size(); i++)
            modelContainers[i]->SetSelected(false);
        selectedContainerIndex = -1;
    }

    // Delete the object from the scene
    if (keys[Qt::Key_Delete])
    {
        if (modelContainers.size() == 0 || selectedContainerIndex == -1)
            return;

        // We keep the model in the map in case they choose to load the same model
        modelContainers.erase(modelContainers.begin() + selectedContainerIndex);
        selectedContainerIndex = -1;
    }

    if (keys[Qt::Key_N])
    {
        // If no model is selected
        if (selectedContainerIndex == -1)
            return;

        int currentWall = modelContainers[selectedContainerIndex]->GetBoundedWall();

        // If the model isn't currently bound to any wall
        if (currentWall < 0 || currentWall >= room->GetWalls().size())
            return;

        Location nextWall = static_cast<Location>((currentWall + 1) % room->GetWalls().size());
        room->BindToWall(modelContainers[selectedContainerIndex].get(), nextWall);
    }

    if (keys[Qt::Key_B])
    {
        if (modifiers[SHIFT])
            drawAABoundingBox = !drawAABoundingBox;
        else
            drawBoundingBox = !drawBoundingBox;
    }

    if (keys[Qt::Key_L])
    {
        if (modifiers[SHIFT])
            showWorldAxis = !showWorldAxis;
        else
            showLocalAxis = !showLocalAxis;
    }

    if (keys[Qt::Key_R])
    {
        if (modifiers[SHIFT] && selectedContainerIndex != -1)
            modelContainers[selectedContainerIndex]->Reset();
        else
        {
            camera = make_unique<Camera>(initialCameraPosition);
            camera->ProcessMouseMovement(initialCameraDelta.x, initialCameraDelta.y);
        }
    }

    if (keys[Qt::Key_Left])
        camera->ProcessMouseMovement(-1, 0);
    if (keys[Qt::Key_Right])
        camera->ProcessMouseMovement(+1, 0);
    if (keys[Qt::Key_Up])
        camera->ProcessMouseMovement(0, +1);
    if (keys[Qt::Key_Down])
        camera->ProcessMouseMovement(0, -1);
    if (keys[Qt::Key_W])
        camera->ProcessKeyboard(modifiers[SHIFT] ? UP : FORWARD, cameraMoveSpeed);
    if (keys[Qt::Key_S])
        camera->ProcessKeyboard(modifiers[SHIFT] ? DOWN : BACKWARD, cameraMoveSpeed);
    if (keys[Qt::Key_A])
        camera->ProcessKeyboard(LEFT, cameraMoveSpeed);
    if (keys[Qt::Key_D])
        camera->ProcessKeyboard(RIGHT, cameraMoveSpeed);

    emit StatusUpdated(drawBoundingBox, drawAABoundingBox, showWorldAxis);
}
