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

//Camera const for scene view rotation will rotate by this angle
const GLfloat ROTATION_ANGLE = 2.0f;
//Camera will rotate around a Scene Point, this is the default position
const glm::vec3 DEFAULT_SCREEN_CENTER = glm::vec3(0.0f,0.0f,0.0f);

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

	//camera scene center for rotations around the center
	glm::vec3 SceneCenter;

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
        Zoom(ZOOM),
		SceneCenter(DEFAULT_SCREEN_CENTER)
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
        Zoom(ZOOM),
		SceneCenter(DEFAULT_SCREEN_CENTER)
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


	/*
	Camera rotates around the scene
	*/
	void ProcessMouseCameraViewRotation(GLfloat xOffset, GLfloat yOffset)
	{
		glm::vec4 newPosition;
		glm::mat4 trans;

		GLfloat angle = ROTATION_ANGLE;

		newPosition = glm::vec4(Position,0.0f);

		//If xOffset is positive than rotate to the right
		if (xOffset > 0)
			angle = -angle;

		//Translate to scene center
		trans = glm::translate(trans, -SceneCenter);
		//rotate based on preset angle
		trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
		//translate back to original point
		trans = glm::translate(trans, SceneCenter);

		//Do transfomation
		newPosition = trans * newPosition;
		
		//Apply transformation onto Camera Position
		Position = glm::vec3(newPosition.x,newPosition.y,newPosition.z);

		//Rotate the camera view to keep facing the Scene center
		Yaw = Yaw - angle;

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
