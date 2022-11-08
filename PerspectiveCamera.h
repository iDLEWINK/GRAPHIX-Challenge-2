#pragma once
#include "MyCamera.h"

class PerspectiveCamera : public MyCamera 
{
	private:
		glm::mat4 projection_matrix;
		float aspectRatio;

	public:	
		PerspectiveCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 WorldUp, float yaw, float pitch, float sensitivity, float screenWidth, float screenHeight)
		: MyCamera(cameraPos, cameraCenter, WorldUp, yaw, pitch, sensitivity, screenWidth, screenHeight)		
		{
			aspectRatio = screenWidth / screenHeight;
			/* Default Settings to initialize the projection matrix */
			projection_matrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
		}

		/* OVERLOAD (Default aspect ratio as per the values passed in the constructor)
			fov - FOV in 
			near - Z Near
			far - Z Far
		*/
		void setProjectionMatrix(float fov, float near, float far) {			
			projection_matrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);
		}

		/* OVERLOAD (Includes an aspect ratio among the arguments; Forces a new aspect ratio)
			fov - FOV in degrees
			ratio - Aspect ratio
			near - Z Near
			far - Z Far
		*/
		void setProjectionMatrix(float fov, float near, float far, float ratio) {
			projection_matrix = glm::perspective(glm::radians(fov), ratio, near, far);
		}

		glm::mat4 getProjectionMatrix() {
			return projection_matrix;
		}
};


