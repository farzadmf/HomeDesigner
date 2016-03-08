#pragma once

#include "Includes.h"

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

const GLfloat YAW           = -90.0f;
const GLfloat PITCH         = 0.0f;
const GLfloat SPEED         = 3.0f;
const GLfloat SENSITIVITY   = 0.25f;
const GLfloat ZOOM          = 45.0f;

const GLfloat MAXIMUM_PITCH = 89.0f;
const GLfloat MAXIMUM_ZOOM  = 90.0f;
const GLfloat MINIMUM_ZOOM  = 30.0f;

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    GLfloat Yaw;
    GLfloat Pitch;

    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw = YAW,
           GLfloat pitch = PITCH) :
        Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        UpdateCameraVectors();
    }

    Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
           GLfloat upX, GLfloat upY, GLfloat upZ,
           GLfloat yaw, GLfloat pitch) :
        Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        Up(0.0f, 1.0f, 0.0f),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        UpdateCameraVectors();
    }

    glm::mat4 GetViewMatrix() const
    {
        return lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(CameraMovement direction, GLfloat deltaTime)
    {
        GLfloat velocity = MovementSpeed * deltaTime;

        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == UP)
            Position.y += velocity;
        if (direction == DOWN)
            Position.y -= velocity;
    }

    void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset,
                              GLboolean constraintPitch = true)
    {
        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;

        Yaw += xOffset;
        Pitch += yOffset;

        if (constraintPitch)
        {
            if (Pitch > MAXIMUM_PITCH)
                Pitch = MAXIMUM_PITCH;
            if (Pitch < -MAXIMUM_PITCH)
                Pitch = -MAXIMUM_PITCH;
        }

        UpdateCameraVectors();
    }

    void ProcessMouseScroll(GLfloat yOffset)
    {
        if (Zoom >= MINIMUM_ZOOM && Zoom <= MAXIMUM_ZOOM)
            Zoom -= yOffset;
        if (Zoom < MINIMUM_ZOOM)
            Zoom = MINIMUM_ZOOM;
        if (Zoom > MAXIMUM_ZOOM)
            Zoom = MAXIMUM_ZOOM;
    }

private:
    void UpdateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
        front.y = sin(glm::radians(Pitch));
        front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
        Front = normalize(front);

        Right = normalize(cross(Front, WorldUp));
        Up = normalize(cross(Right, Front));
    }
};
