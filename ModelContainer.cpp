#include "ModelContainer.h"
#include "CollisionManager.h"

glm::mat4 ModelContainer::CombineTransformations() const
{
    glm::mat4 modelMatrix;
    modelMatrix = translate(modelMatrix, initialTranslateVector + translateVector);
    modelMatrix = rotate(modelMatrix, glm::radians(initialRotationAngles.x) + glm::radians(rotationAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = rotate(modelMatrix, glm::radians(initialRotationAngles.y) + glm::radians(rotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = rotate(modelMatrix, glm::radians(initialRotationAngles.z) + glm::radians(rotationAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    return modelMatrix;
}

void ModelContainer::UpdateBoundingBox()
{
    vector<glm::vec3> boundingBoxVertices = model->GetBoundingBoxVertices();
    for (int i = 0; i < boundingBoxVertices.size(); i++)
    {
        glm::mat4 modelMatrix = CombineTransformations();
        glm::vec4 transformedPoint = modelMatrix * glm::vec4(boundingBoxVertices[i], 1.0f);
        aaBoundingBoxVertices[i].x = transformedPoint.x;
        aaBoundingBoxVertices[i].y = transformedPoint.y;
        aaBoundingBoxVertices[i].z = transformedPoint.z;
    }

    vector<GLfloat> allX, allY, allZ;
    for (auto vertex : aaBoundingBoxVertices)
    {
        allX.push_back(vertex.x);
        allY.push_back(vertex.y);
        allZ.push_back(vertex.z);
    }

    GLfloat minX, minY, minZ, maxX, maxY, maxZ;
    minX = *min_element(allX.begin(), allX.end());
    maxX = *max_element(allX.begin(), allX.end());
    minY = *min_element(allY.begin(), allY.end());
    maxY = *max_element(allY.begin(), allY.end());
    minZ = *min_element(allZ.begin(), allZ.end());
    maxZ = *max_element(allZ.begin(), allZ.end());

    aaBoundingBoxVertices = vector<glm::vec3>();
    aaBoundingBoxVertices.push_back(glm::vec3(minX, minY, minZ));    // 1
    aaBoundingBoxVertices.push_back(glm::vec3(minX, minY, maxZ));    // 2
    aaBoundingBoxVertices.push_back(glm::vec3(minX, maxY, maxZ));    // 3
    aaBoundingBoxVertices.push_back(glm::vec3(minX, maxY, minZ));    // 4
    aaBoundingBoxVertices.push_back(glm::vec3(maxX, minY, minZ));    // 5
    aaBoundingBoxVertices.push_back(glm::vec3(maxX, maxY, minZ));    // 6
    aaBoundingBoxVertices.push_back(glm::vec3(maxX, maxY, maxZ));    // 7
    aaBoundingBoxVertices.push_back(glm::vec3(maxX, minY, maxZ));    // 8

	//Compute container center
	modelContainerCenter = glm::vec3( (maxX +minX)/2.0f, (maxY + minY) /2.0f, (maxZ + minZ) /2.0f);
}

void ModelContainer::DetectCollision()
{
    colliding = CollisionManager::GetInstance().IsColliding(this, collisionManagerIndex);
}

ModelContainer::ModelContainer(Model* model, GLfloat initialScale, Room* room, QOpenGLWidget* targetWidget) :
    model{ model }, scaleFactor{ initialScale }, initialScale{ initialScale }, targetWidget{ targetWidget },
    room{ room }
{
    aaBoundingBoxVertices = model->GetBoundingBoxVertices();

    // Give the model an initial offset in the 'y' direction (in case it's located exactly on the floor)
    SetInitialTranslateVector(glm::vec3(0.0f, 0.001f, 0.0f));

    UpdateBoundingBox();

    if (IsInsideRoom())
    {
        collisionManagerIndex = CollisionManager::GetInstance().AddContainer(this);
        DetectCollision();
    }
}

ModelContainer::~ModelContainer()
{
    CollisionManager::GetInstance().RemoveContainer(this);
}

void ModelContainer::SetInitialScale(GLfloat initialScale)
{
    this->initialScale = initialScale;
    UpdateBoundingBox();
    DetectCollision();
}

void ModelContainer::SetInitialTranslateVector(glm::vec3 initialTranslateVector)
{
    this->initialTranslateVector = initialTranslateVector;
    UpdateBoundingBox();
    DetectCollision();
}

void ModelContainer::SetInitialRotationAngles(glm::vec3 initialRotationAngles)
{
    this->initialRotationAngles = initialRotationAngles;
    UpdateBoundingBox();
    DetectCollision();
}

void ModelContainer::ScaleBy(GLfloat scaleFactor)
{
    bool operationAllowed = true;
    QString reason;     // Message indicating why the operation wasn't successful

    auto currentScaleFactor = this->scaleFactor;

    // Try to scale and see whether it gets out of the room
    this->scaleFactor = scaleFactor;
    UpdateBoundingBox();
    DetectCollision();

    // If not inside the room, revert back the changes
    if (!IsInsideRoom())
    {
        this->scaleFactor = currentScaleFactor;
        UpdateBoundingBox();
        DetectCollision();
        operationAllowed = false;
        reason = "The object is getting out of the room!";
    }

    if (!operationAllowed)
        emit OperationNotAllowed("<b><font color='red'>Cannot scale! " + reason + "</font></b>");
    else
        emit OperationSuccessful();
}

void ModelContainer::RotateBy(glm::vec3 angles)
{
    bool operationAllowed = true;
    QString reason;     // Message indicating why the operation wasn't successful

    auto currentRotationAngles = rotationAngles;

    // Try to rotate and see whether it gets out of the room
    rotationAngles.x += rotationBound.x * angles.x;
    rotationAngles.y += rotationBound.y * angles.y;
    rotationAngles.z += rotationBound.z * angles.z;
    UpdateBoundingBox();
    DetectCollision();

    if ((angles.x != 0 && rotationBound.x == 0) ||
        (angles.y != 0 && rotationBound.y == 0) ||
        (angles.z != 0 && rotationBound.z == 0))
    {
        operationAllowed = false;
        reason = "The object's rotation is bounded in this direction!";
    }

    // If model's box is not inside the room, revert back the changes
    if (!IsInsideRoom())
    {
        rotationAngles = currentRotationAngles;
        UpdateBoundingBox();
        DetectCollision();
        operationAllowed = false;
        reason = "The object is getting out of the room!";
    }

    if (!operationAllowed)
        emit OperationNotAllowed("<b><font color='red'>Cannot rotate! " + reason + "</font></b>");
    else
        emit OperationSuccessful();
}

void ModelContainer::TranslateBy(glm::vec3 translateVector)
{
    bool operationAllowed = true;
    QString reason;     // Message indicating why the operation wasn't successful

    auto currentTranslateVector = this->translateVector;

    // Try to move and see whether it gets out of the room
    this->translateVector.x += translationBound.x * translateVector.x;
    this->translateVector.y += translationBound.y * translateVector.y;
    this->translateVector.z += translationBound.z * translateVector.z;
    UpdateBoundingBox();
    DetectCollision();

    if ((translateVector.x != 0 && translationBound.x == 0) ||
        (translateVector.y != 0 && translationBound.y == 0) ||
        (translateVector.z != 0 && translationBound.z == 0))
    {
        operationAllowed = false;
        reason = "The object's movement is bounded in this direction!";
    }

    // If out of the room, revert back the changes
    if (!IsInsideRoom())
    {
        this->translateVector = currentTranslateVector;
        UpdateBoundingBox();
        DetectCollision();
        operationAllowed = false;
        reason = "The object is getting out of the room!";
    }

    if (!operationAllowed)
        emit OperationNotAllowed("<b><font color='red'>Cannot move! " + reason + "</font></b>");
    else
        emit OperationSuccessful();
}

bool ModelContainer::SetSelected(bool selected)
{
    if (!selectable)
        return false;

    this->selected = selected;
    return true;
}

void ModelContainer::DrawModel(Shader& shader, Shader& outlineShader, int index)
{
    auto& finalShader = colliding ? outlineShader : shader;
    finalShader.Use();

    if (colliding)
    {
        glUniform1i(glGetUniformLocation(finalShader.GetProgram(), "modelOverride"), false);
        glUniform3f(glGetUniformLocation(finalShader.GetProgram(), "fragmentColor"), 0.5f, 0.0f, 0.0f);
    }

    auto modelMatrix = CombineTransformations();
    glUniformMatrix4fv(glGetUniformLocation(finalShader.GetProgram(), "model"), 1, GL_FALSE, value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(finalShader.GetProgram(), "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(finalShader.GetProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));

    // Update the stencil buffer when drawing the model itself
    glStencilFunc(GL_ALWAYS, index, 0xFF);
    glStencilMask(0xFF);

    model->Draw(shader);

    if (selected)
    {
        // Don't draw when stencil test fails
        glStencilFunc(GL_NOTEQUAL, index, 0xFF);
        glStencilMask(0x00);

        outlineShader.Use();
        glUniform3f(glGetUniformLocation(outlineShader.GetProgram(), "fragmentColor"), 0.0f, 1.0f, 1.0f);
        glUniform1i(glGetUniformLocation(outlineShader.GetProgram(), "modelOverride"), false);
        glUniformMatrix4fv(glGetUniformLocation(outlineShader.GetProgram(), "view"), 1, GL_FALSE, value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(outlineShader.GetProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));
        model->DrawOutline(modelMatrix, outlineShader);

        // Revert back default stencil buffer settings
        extern int defaultStencilValue;
        glStencilFunc(GL_ALWAYS, defaultStencilValue, 0xFF);
        glStencilMask(0xFF);
    }
}

void ModelContainer::DrawModelBoundingBox(Shader& shader)
{
    shader.Use();

    glUniform3f(glGetUniformLocation(shader.GetProgram(), "fragmentColor"), 0.8f, 0.0f, 0.0f);
    glm::mat4 modelMatrix = CombineTransformations();
    glUniform1i(glGetUniformLocation(shader.GetProgram(), "modelOverride"), false);
    glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "model"), 1, GL_FALSE, value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));

    model->DrawBoundingBox();
}

void ModelContainer::DrawAABoudningBox(Shader& shader)
{
    shader.Use();

    glUniform3f(glGetUniformLocation(shader.GetProgram(), "fragmentColor"), 0.2f, 0.8f, 0.8f);
    glUniform1i(glGetUniformLocation(shader.GetProgram(), "modelOverride"), true);
    glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));

    model->DrawBoundingBox(aaBoundingBoxVertices);
}

void ModelContainer::Reset()
{
    translateVector = glm::vec3(0.0f, 0.0f, 0.0f);
    rotationAngles = glm::vec3(0.0f);
    scaleFactor = initialScale;
    UpdateBoundingBox();
    DetectCollision();
}

bool ModelContainer::IsInsideRoom() const
{
    auto roomMinimumVertices = room->GetMinimumCoordinates();
    auto roomMaximumVertices = room->GetMaximumCoordinates();
    auto minimumVertices = GetMinimumVertices();
    auto maximumVertices = GetMaximumVertices();

    // Debugging information
//    cout << "room min: " << roomMinimumVertices.x << ", " << roomMinimumVertices.y << ", " << roomMinimumVertices.z << endl;
//    cout << "room max: " << roomMaximumVertices.x << ", " << roomMaximumVertices.y << ", " << roomMaximumVertices.z << endl;
//    cout << "min: " << minimumVertices.x << ", " << minimumVertices.y << ", " << minimumVertices.z << endl;
//    cout << "max: " << maximumVertices.x << ", " << maximumVertices.y << ", " << maximumVertices.z << endl;
//
//    cout << "!(minimumVertices.x > roomMinimumVertices.x): " << !(minimumVertices.x > roomMinimumVertices.x) << endl;
//    cout << "!(minimumVertices.y > roomMinimumVertices.y): " << !(minimumVertices.y > roomMinimumVertices.y) << endl;
//    cout << "!(minimumVertices.z > roomMinimumVertices.z): " << !(minimumVertices.z > roomMinimumVertices.z) << endl;
//    cout << "!(maximumVertices.x < roomMaximumVertices.x): " << !(maximumVertices.x < roomMaximumVertices.x) << endl;
//    cout << "!(maximumVertices.y < roomMaximumVertices.y): " << !(maximumVertices.y < roomMaximumVertices.y) << endl;
//    cout << "!(maximumVertices.z < roomMaximumVertices.z): " << !(maximumVertices.z < roomMaximumVertices.z) << endl;

    return minimumVertices.x > roomMinimumVertices.x &&
        minimumVertices.y > roomMinimumVertices.y &&
        minimumVertices.z > roomMinimumVertices.z &&
        maximumVertices.x < roomMaximumVertices.x &&
        maximumVertices.y < roomMaximumVertices.y &&
        maximumVertices.z < roomMaximumVertices.z;
}
