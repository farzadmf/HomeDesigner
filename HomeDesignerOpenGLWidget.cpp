#include "HomeDesignerOpenGLWidget.h"
#include <QMouseEvent>
#include <QCoreApplication>

using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;

static int lastMouseX;                              // Last X position of the mouse
static int lastMouseY;                              // Last Y position of the mouse
static map<int, bool> keys;                         // Map representing currently pressed keys
static bool modifiers[3];                           // Modifier keys (SHIFT, CTRL, and ALT)
static GLfloat cameraMoveSpeed = 0.2f;              // Offset speed used for camera movement
static bool firstMouseMovement = true;              // Is this first time the mouse is moving?

static bool drawBoundingBox = false;                // Whether to display bounding boxes of models
static bool drawAABoundingBox = false;              // Whether to display AA bounding boxes of the models
static bool showLocalAxis = false;                  // Display model's local axis?
static bool showWorldAxis = false;                  // Display world axis?

static GLuint axisVao, axisVbo, axisEbo;            // Array and buffer objects used to draw the axes
static Shader axisShader;                           // Shader used to draw the axes

static glm::vec3 initialCameraPosition(0.0f, 41.0f, 65.0f);
static glm::vec2 initialCameraDelta(0.0f, -110.0f); // Needed for Camera class's delta value

//room initial settings
static GLfloat roomWidth = 60.0f;
glm::vec3 initialWallColor(0.4f);
glm::vec3 initialFloorColor(0.2f);

int defaultStencilValue = 1;                        // Default value written to stencil buffer

HomeDesignerOpenGLWidget::HomeDesignerOpenGLWidget(QWidget* parent) :
    QOpenGLWidget(parent)
{
    timer.start();
}

HomeDesignerOpenGLWidget::~HomeDesignerOpenGLWidget()
{
}

/**
* Main loop of the application
**/
void HomeDesignerOpenGLWidget::paintGL()
{
    // We capture keyboard state in 'keyboardEvent' and call this function to process the keys.
    ProcessKeyboard();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Create view and projection matrices
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom),
                                            static_cast<GLfloat>(width()) / static_cast<GLfloat>(height()), 0.1f, 1000.0f);

    axisShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(axisShader.GetProgram(), "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(axisShader.GetProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));

    // Display world axis
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

    for (int index = 0; index < modelContainers.size(); index++)
    {
        auto container = modelContainers[index].get();

        container->SetProjectionMatrix(projection);
        container->SetViewMatrix(view);
        container->DrawModel(*shader, *outlineShader, index + 1);
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

    if (collisionDetected)
        EmitDisplayMessage("<b><font color='red'>COLLISION DETECTED!!! Please resolve!!!</font></b>", 0);
}

/**
* Called when the widget is being initialized
**/
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

    // Set required OpenGL states
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, defaultStencilValue, 0xFF);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Create the data for the axes
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

    // Buffer axis data to the GPU
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

    // Initialize required objects
    shader = make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
    shader->Create();
    outlineShader = make_shared<Shader>("shaders/outline.vs", "shaders/outline.fs");
    outlineShader->Create();
    camera = make_unique<Camera>(initialCameraPosition);
    camera->ProcessMouseMovement(initialCameraDelta.x, initialCameraDelta.y);
    camera->MouseSensitivity = 0.5f;
    camera->MovementSpeed = 5.0f;
    room = make_shared<Room>(this, roomWidth, initialWallColor, initialFloorColor);
}

/**
* Process mouse movement
**/
void HomeDesignerOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    // Move the camera if right mouse button is down and scene isn't empty
    if ((rightMouseButtonDown || middleMouseButtonDown) && modelContainers.size() > 0)
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

        // If right mouse button is pressed, look
        if (rightMouseButtonDown)
            camera->ProcessMouseMovement(xOffset, yOffset);
        else if (middleMouseButtonDown)
        {
            // In case of middle click, pan
            if (xOffset > 0) camera->ProcessKeyboard(RIGHT, cameraMoveSpeed);
            if (xOffset < 0) camera->ProcessKeyboard(LEFT, cameraMoveSpeed);
            if (yOffset > 0) camera->ProcessKeyboard(UP, cameraMoveSpeed);
            if (yOffset < 0) camera->ProcessKeyboard(DOWN, cameraMoveSpeed);
        }

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

/**
* Process key press event
**/
void HomeDesignerOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    // Keep track of pressed keys and modifiers
    keys[event->key()] = true;
    modifiers[SHIFT] = event->modifiers() & Qt::ShiftModifier;
    modifiers[CONTROL] = event->modifiers() & Qt::ControlModifier;
    modifiers[ALT] = event->modifiers() & Qt::AltModifier;

    // Select the operation axis and display a text accordingly
    Axis oldAxis = axis;
    switch (event->key())
    {
        case Qt::Key_X:
            axis = X;
            if (oldAxis != axis)
                EmitDisplayMessage("<b><font color='maroon'>Axis   -------->   x  ( MAROON )</font></b>", 0);
            break;

        case Qt::Key_Y:
            axis = Y;
            if (oldAxis != axis)
                EmitDisplayMessage("<b><font color='green'>Axis   -------->   y  ( GREEN )</font></b>", 0);
            break;

        case Qt::Key_Z:
            axis = Z;
            if (oldAxis != axis)
                EmitDisplayMessage("<b><font color='blue'>Axis   -------->   z  ( BLUE )</font></b>", 0);
            break;

        default:
            axis = NONE;
            EmitClearMessage();
            break;
    }

    // Quit application with 'Q'
    if (event->key() == Qt::Key_Q)
        emit Exit();

    update();
}

/**
* Reset pressed keys and modifiers, along with status bar message
**/
void HomeDesignerOpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    // If this is an auto-repeat of the key, do nothing
    if (event->isAutoRepeat())
        return;

    axis = NONE;
    EmitClearMessage();
    keys[event->key()] = false;
    modifiers[SHIFT] = event->modifiers() & Qt::ShiftModifier;
    modifiers[CONTROL] = event->modifiers() & Qt::ControlModifier;
    modifiers[ALT] = event->modifiers() & Qt::AltModifier;
}

/**
* Keep track of mouse button(s) being pressed
**/
void HomeDesignerOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    leftMouseButtonDown = event->button() == Qt::LeftButton;
    rightMouseButtonDown = event->button() == Qt::RightButton;
    middleMouseButtonDown = event->button() == Qt::MiddleButton;

    // Grab mouse and change cursor to hand in case of right and middle clicks
    if (rightMouseButtonDown || middleMouseButtonDown)
        grabMouse(Qt::ClosedHandCursor);
}

/**
* Reset mouse buttons and release mouse
**/
void HomeDesignerOpenGLWidget::mouseReleaseEvent(QMouseEvent*)
{
    leftMouseButtonDown = rightMouseButtonDown = middleMouseButtonDown = false;
    firstMouseMovement = true;
    releaseMouse();
}

/**
* Zoom in/out with mouse wheel
**/
void HomeDesignerOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    camera->ProcessMouseScroll(event->delta() > 0 ? 1 : -1);
    update();
}

/**
* Called when the desired operation changes in the main window
**/
void HomeDesignerOpenGLWidget::OnMoveSelectedChanged(bool selected) { isMove = selected; }
void HomeDesignerOpenGLWidget::OnRotateSelectedChanged(bool selected) { isRotate = selected; }
void HomeDesignerOpenGLWidget::OnScaleSelectedChanged(bool selected) { isScale = selected; }

/**
* Called when the operation's speed changes in the main window
**/
void HomeDesignerOpenGLWidget::OnMoveSpeedChanged(int speed) { moveSpeed = speed; }
void HomeDesignerOpenGLWidget::OnRotateSpeedChanged(int speed) { rotateSpeed = speed; }
void HomeDesignerOpenGLWidget::OnScaleSpeedChanged(int speed) { scaleSpeed = speed; }

/**
* Loads a model from the file, setting its initial scale value
**/
void HomeDesignerOpenGLWidget::OnLoadModel(QString modelAttributes, GLfloat initialScale)
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
        models[modelPath] = make_unique<Model>(this, modelPath);
        message += "DONE!";
        emit DisplayMessage(QString::fromStdString(message), 3000);
    }

    // Create the model container
    auto container = make_unique<ModelContainer>(models[modelPath].get(), initialScale, room.get(), this);

    // If the model doens't fit the room
    if (!container->IsInsideRoom())
    {
        emit DisplayError("The model object doesn't fit in the room! Please try again.");
        container.release();
        return;
    }

    // Select newly selected model container
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
            // Location of the wall to be bound to
            // If no location is specified, default wall is the left wall
            Location wallLocation;
            if (attributes.size() == 2)
                wallLocation = Wall::GetLocationByName("left");
            else
                wallLocation = Wall::GetLocationByName(attributes[2].toStdString());

            room->BindToWall(container.get(), wallLocation);
        }
    }

    // Deselect other containers in the scene
    for (int i = 0; i < modelContainers.size(); i++)
        modelContainers[i]->SetSelected(false);

    modelContainers.push_back(std::move(container));
    selectedContainerIndex = modelContainers.size() - 1;
    grabKeyboard();
    update();
}

// Displays a message when the operation (move, rotate, scale) isn't allowed
void HomeDesignerOpenGLWidget::OnOperationNotAllowed(QString message)
{
    emit DisplayMessage(message, 0);
}

/**
* Restores the previous message in the status bar when the operation is successful
**/
void HomeDesignerOpenGLWidget::OnOperationSuccessful()
{
    emit DisplayMessage(lastMessage, 0);
}

/**
* Changes the wall color for the room
**/
void HomeDesignerOpenGLWidget::OnChangeRoomWallColor(QColor color) const
{
    room->SetWallColor(glm::vec3(color.redF(), color.greenF(), color.blueF()));
}

/**
* Changes the wall texture
**/
void HomeDesignerOpenGLWidget::OnChangeRoomWallTexture(QString textureFilePath) const
{
    room->SetWallTexture(textureFilePath.toStdString());
}

/**
* Changes the floor color
**/
void HomeDesignerOpenGLWidget::OnChangeRoomFloorColor(QColor color) const
{
    room->SetFloorColor(glm::vec3(color.redF(), color.greenF(), color.blueF()));
}

/**
* Changes the floor texture
**/
void HomeDesignerOpenGLWidget::OnChangeRoomFloorTexture(QString textureFilePath) const
{
    room->SetFloorTexture((textureFilePath.toStdString()));
}

/**
* Processes keyboard input
**/
void HomeDesignerOpenGLWidget::ProcessKeyboard()
{
    // Space key
    if (keys[Qt::Key_Space])
    {
        // If collision is detected, we don't do anything
        if (collisionDetected || modelContainers.size() == 0)
            return;

        // Otherwise, select the next item
        while (true)
        {
            if (selectedContainerIndex != -1)
                modelContainers[selectedContainerIndex]->SetSelected(false);

            selectedContainerIndex = (selectedContainerIndex + 1) % modelContainers.size();
            if (modelContainers[selectedContainerIndex]->SetSelected(true))
                break;
        }
    }

    // Escape key
    if (keys[Qt::Key_Escape])
    {
        // Don't do anything if collision is detected
        if (collisionDetected)
            return;

        // Deselect everything
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

    // Move the wall-bounded object to the next wall
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

    // Display bounding box (or AA bounding box in case SHIFT is pressed)
    if (keys[Qt::Key_B])
    {
        if (modifiers[SHIFT])
            drawAABoundingBox = !drawAABoundingBox;
        else
            drawBoundingBox = !drawBoundingBox;
    }

    // Display world axis (or object's local one if SHIFT is pressed)
    if (keys[Qt::Key_L])
    {
        if (modifiers[SHIFT])
            showWorldAxis = !showWorldAxis;
        else
            showLocalAxis = !showLocalAxis;
    }

    // Reset the camera (or the object if SHIFT is pressed)
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

    // Camera movements:
    //      Left arrow : look to the left
    //      Right arrow: look to the right
    //      Up arrow   : look up
    //      Down arrow : look down
    //      'W'        : move forward
    //      SHIFT + 'W': move up
    //      'S'        : move backwards
    //      SHIFT + 'S': move down
    //      'A'        : strafe left
    //      'D'        : strafe right
    if (keys[Qt        ::Key_Left])
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

/**
* Helper function to display a message in the status bar
**/
void HomeDesignerOpenGLWidget::EmitDisplayMessage(QString message, int timeout)
{
    lastMessage = message;
    emit DisplayMessage(lastMessage, timeout);
}

/**
* Helper function to clear the message in the status bar
**/
void HomeDesignerOpenGLWidget::EmitClearMessage()
{
    lastMessage = "";
    emit ClearMessage();
}
