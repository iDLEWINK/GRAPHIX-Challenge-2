#include "MyCamera.h"
#pragma once

class PerspectiveCamera : public MyCamera 
{
	private:
		float aspectRatio, distance;
	public:	
		PerspectiveCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 WorldUp, float yaw, float pitch, float screenWidth, float screenHeight)
			: MyCamera(cameraPos, cameraCenter, WorldUp, yaw, pitch)		
		{
			aspectRatio = screenWidth / screenHeight;

			/* Initialize Camera Movement Variable */						
			distance = cameraPos.z; // Z is the original distance position of the camera from the center
			
			/* Default Settings to initialize the projection matrix */
			glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
			/* Set projection_matrix of parent */
			MyCamera::setProjectionMatrix(projection_matrix);
		}
		
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

			MyCamera::updateCameraPos(distance);
		}



		/* OVERLOAD (Default aspect ratio as per the values passed in the constructor)
			fov - FOV in 
			near - Z Near
			far - Z Far
		*/
		void setProjectionMatrix(float fov, float near, float far) {			
			glm::mat4 projection_matrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);
			MyCamera::setProjectionMatrix(projection_matrix);
		}

		/* OVERLOAD (Includes an aspect ratio among the arguments; Forces a new aspect ratio)
			fov - FOV in degrees
			ratio - Aspect ratio
			near - Z Near
			far - Z Far
		*/
		void setProjectionMatrix(float fov, float near, float far, float ratio) {
			glm::mat4 projection_matrix = glm::perspective(glm::radians(fov), ratio, near, far);
			MyCamera::setProjectionMatrix(projection_matrix);
		}

		glm::mat4 getProjectionMatrix() {
			return MyCamera::getProjectionMatrix();
		}
};


