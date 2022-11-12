#include "MyCamera.h"
#pragma once

class PerspectiveCamera : public MyCamera 
{
	private:
		float aspectRatio, lastX, lastY, sensitivity;

	public:	
		PerspectiveCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 WorldUp, float yaw, float pitch, float sensitivity, float screenWidth, float screenHeight)
			: MyCamera(cameraPos, cameraCenter, WorldUp, yaw, pitch)		
		{
			aspectRatio = screenWidth / screenHeight;
			lastX = screenWidth / 2.0;
			lastY = screenHeight / 2.0;
			this->sensitivity = sensitivity;

			
			/* Default Settings to initialize the projection matrix */
			glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
			/* Set projection_matrix of parent */
			MyCamera::setProjectionMatrix(projection_matrix);
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

		void setLastX(float lastX) {
			this->lastX = lastX;
		}

		void setLastY(float lastY) {
			this->lastY = lastY;
		}

		float getLastX() {
			return lastX;
		}

		float getLastY() {
			return lastY;
		}
};


