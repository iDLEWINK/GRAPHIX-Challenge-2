#include "MyCamera.h"
#pragma once

/* Perspective camera derived class by inheriting base class MyCamera */
class PerspectiveCamera : public MyCamera 
{
	private:
		float aspectRatio, distance;
	public:	
		/* Perspective camera constructor */
		PerspectiveCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 WorldUp, float screenWidth, float screenHeight)
			: MyCamera(cameraPos, cameraCenter, WorldUp)		
		{
			/* Compute for the aspect ratio for computing for perspective projection matrix */
			aspectRatio = screenWidth / screenHeight;		
			/* Z is the original distance position of the camera from the center; Initialize Camera Movement Variable */
			distance = cameraPos.z; 
						
			/* Set the value of the projection_matrix attribute of MyCamera base class */
			/* Default settings to initialize the perspective projection matrix */
			MyCamera::setProjectionMatrix(glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f));
		}
		
		/* Updates the yaw and pitch based upon the current offsets and updates the camera position */
		void updateMouse(float xoffset, float yoffset) {
			float yaw = MyCamera::getYaw();
			float pitch = MyCamera::getPitch();

			/* Update the angles through adding of offsets */
			yaw = glm::mod(yaw + xoffset, 360.0f); // Mod for safety so that yaw's value do not overflow
			pitch += yoffset;

			/* Sets the constraints for vertical look movement (MAX ANGLE: straight up and straight down ) */
			if (pitch > 89.0f) {
				pitch = 89.0f;
			}
			if (pitch < -89.0f) {
				pitch = -89.0f;
			}

			/*
				Set the updated position values (Pythagorean Theorem)
				yaw - x-axis (cos) and z-axis (sin)
				pitch - x-axis (cos), z-axis (cos), y-axis(sin)
			*/
			MyCamera::setYaw(yaw);
			MyCamera::setPitch(pitch);
			/* Update the camera position with respect to the multiplier (distance in this case) */
			MyCamera::updateCameraPos(distance);
		}

		/* Perspective camera projection matrix getter and setter */
		/* OVERLOAD (Default aspect ratio as per the values passed in the constructor)
			fov - FOV in degrees
			near - Z Near
			far - Z Far
		*/
		void setProjectionMatrix(float fov, float near, float far) {			
			/* Set the value of the projection_matrix attribute of MyCamera base class */
			MyCamera::setProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, near, far));
		}

		/* OVERLOAD (Includes an aspect ratio among the arguments; Forces a new aspect ratio)
			fov - FOV in degrees
			ratio - Aspect ratio
			near - Z Near
			far - Z Far
		*/
		void setProjectionMatrix(float fov, float near, float far, float ratio) {
			/* Set the value of the projection_matrix attribute of MyCamera base class */
			MyCamera::setProjectionMatrix(glm::perspective(glm::radians(fov), ratio, near, far));
		}

		glm::mat4 getProjectionMatrix() {
			return MyCamera::getProjectionMatrix();
		}
};


