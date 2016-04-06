#pragma once

#include "Includes.h"
// camera movement functionalities
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
const glm::vec3 DEFAULT_SCREEN_CENTER    = glm::vec3(0.0f,0.0f,0.0f);

///
/// \brief The Camera class
/// This class provides the functionalities of a camera viewing, allowing the zooming and rotation of a camera.Using the position 
// pitch and yaw.
///
class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    glm::vec3 ViewDirection;

    //camera scene center for rotations around the center
    glm::vec3 SceneCenter;

    GLfloat Yaw;
    GLfloat Pitch;

    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;
    ///
    /// \brief Camera Constructor to enable the functionalities of the camera
    /// \param position the camera position
    /// \param up the position of the up vector
    /// \param yaw the value of yaw
    /// \param pitch the value of pitch
    ///
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
    ///
    /// \brief Camera Camera Constructor to enable the functionalities of the camera
    /// \param posX x coordinate
    /// \param posY y coordinate
    /// \param posZ z coordinae
    /// \param upX up x coordinate
    /// \param upY up y coordinate
    /// \param upZ up z coordinate
    /// \param yaw the yaw value
    /// \param pitch the pitch value
    ///
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
    ///
    /// \brief GetViewMatrix gives the view
    /// \return the view matrix
    ///
    glm::mat4 GetViewMatrix() const
    {
            return lookAt(Position, ViewDirection, Up);
    }
    ///
    /// \brief ProcessKeyboard takes the keyboard input and moves the position of the camera
    /// \param direction the direction to move the camera in
    /// \param deltaTime the time change used to determine the velocity of the position change
    ///
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

        ViewDirection = Position + Front;
    }
    ///
    /// \brief ProcessMouseMovement process the mouse movement using the offset conditions
    /// \param xOffset the x offset
    /// \param yOffset the y offset
    /// \param constraintPitch the pitch constaint
    ///
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
    ///
    /// \brief turnUpsideDown gives the ability to turn the room upside down.
    ///
	void turnUpsideDown() {
		WorldUp.y=Up.y = -Up.y;

		cout << ViewDirection.y;
	}


    /// \brief ProcessMouseCameraViewRotation  Camera rotates around the scene based off the offset it will determine which direction to rotate in
    /// \param xOffset the x offset 
    /// \param yOffset the y offset
    ///
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
        trans = translate(trans, -SceneCenter);
        //rotate based on preset angle
        trans = rotate(trans, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
        //translate back to original point
        trans = translate(trans, SceneCenter);

        //Do transformation
        newPosition = trans * newPosition;
        
        //Apply transformation onto Camera Position
        Position = glm::vec3(newPosition.x,newPosition.y,newPosition.z);

        //Rotate the camera view to keep facing the Scene center
        Yaw = Yaw - angle;

        UpdateCameraVectors();
    }


    ///
    /// \brief RotateToPointOfFocus Move Camera's facing direction to point of focus
    /// \param pointOfFocus vector to rotate point focus
    ///
    void RotateToPointOfFocus(glm::vec3 pointOfFocus)
    {
        auto v1 = normalize(pointOfFocus - Position);   // Vector from camera position to focus point
        auto v2 = normalize(ViewDirection - Position);  // Vector from camera position along its view direction
        auto dotProduct = dot(v1, v2);                  // Angle between the two vectors
        auto angle = glm::degrees(acos(dotProduct));    // Angle in degrees
        auto crossProduct = cross(v1, v2);              // If crossProduct.y > 0, focus is on the right; left otherwise

        ViewDirection = pointOfFocus;
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        Yaw += crossProduct.y > 0 ? angle : -angle;
        UpdateCameraVectors();
    }

	///
	/// \brief Moves camera close to selected object for fast and easy editing of current object
	///
	void getCloseToSelectedObject(glm::vec3 modelCenter, GLfloat maxX, GLfloat minX) {

		//If object is too close to the minX wall then do special action
		if (modelCenter.x < minX + 5.0f) {

			//Camera's position close to selected object
			Position = (glm::vec3(modelCenter.x + 20.0f, modelCenter.y + 15.0f, modelCenter.z));
			Yaw = -180.0f;
		}
		//If object is too close to the maxX wall then do special action
		else if (modelCenter.x > maxX - 5.0f) {

			//Camera's position close to selected object
			Position = (glm::vec3(modelCenter.x - 20.0f, modelCenter.y + 15.0f, modelCenter.z));
			Yaw = 0.0f;
		}
		//else default action
		else {
			//Camera's position close to selected object
			Position = (glm::vec3(modelCenter.x, modelCenter.y + 15.0f, modelCenter.z + 20.0f));
			Yaw = -90.0f;
		}

		Pitch = -35.0f;
		UpdateCameraVectors();
		
	}


	///
	/// \brief Moves camera to give a birds eye view perspective of the scene
	///
	void birdsEyeView(GLfloat maxX, GLfloat minX, GLfloat maxY, GLfloat minY, GLfloat maxZ, GLfloat minZ) {

		    //Camera's bird's eye view position
			Position = (glm::vec3(0.0f, maxY + 80.0f, 0.0f));
			Yaw = 270.0f;
			Pitch = -88.0f;
			UpdateCameraVectors();
	}

	///
	/// \brief Moves camera to give an eye level view of the scene
	///
	void horizontalFaceView(GLfloat maxX, GLfloat minX, GLfloat maxY, GLfloat minY, GLfloat maxZ, GLfloat minZ) {
		    
		   //Camera's Horizontal view position
			Position = (glm::vec3(0.0f, maxY / 2.0f, maxZ + 40.0f));
			Yaw = -90.0f;
			Pitch = 0.0f;
			UpdateCameraVectors();
	}

	///
	/// \brief Cycles through the scenes corners with camera facing the scene center
	///
	void cycleThroughSceneCorners(GLfloat maxX, GLfloat minX, GLfloat maxY, GLfloat minY, GLfloat maxZ, GLfloat minZ) {
		
		//if Front left corner of the scene
		if   (Position == glm::vec3(minX + 1, maxY + 10.0f, maxZ)) {
			//Back left corner of the scene
			Position = (glm::vec3(minX + 1, maxY + 10.0f, minZ + 1));
			Yaw = 45.0f;
		}
		//if Back left corner of the scene
		else if (Position == glm::vec3(minX + 1, maxY + 10.0f, minZ + 1)) {
			//Back right corner of the scene
			Position = (glm::vec3(maxX - 1, maxY + 10.0f, minZ + 1));
			Yaw = 135.0f;
		}
		//if Back right corner of the scene
		else if (Position == glm::vec3(maxX - 1, maxY + 10.0f, minZ + 1)) {
			//Front right corner of the scene
			Position = (glm::vec3(maxX - 1, maxY + 10.0f, maxZ));
			Yaw = 225.0f;
		}
		//if Front right corner of the scene
		else {
			//Front left corner of the scene
			Position = (glm::vec3(minX + 1, maxY + 10.0f, maxZ));
			Yaw = -46.0f;
		}

		Pitch = -45.0f;
		UpdateCameraVectors();
	}

    ///
    /// \brief ProcessMouseScroll proceses the mouse scroll based on if the zoom is within Minimum and maximum zoom
    /// \param yOffset change the zoom based on the offset
    ///
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

        ViewDirection = Position + Front;
    }
};
