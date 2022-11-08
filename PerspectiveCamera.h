#pragma once
#include "MyCamera.h"

class PerspectiveCamera : public MyCamera 
{
	private:
		glm::mat4 projection_matrix;

	public:	
		PerspectiveCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 WorldUp, float yaw, float pitch, float sensitivity, float screenWidth, float screenHeight)
		: MyCamera(cameraPos, cameraCenter, WorldUp, yaw, pitch, sensitivity, screenWidth, screenHeight)		
		{
			/* Default Settings to initialize the projection matrix */
			projection_matrix = glm::perspective(glm::radians(45.0f), screenWidth/screenHeight, 0.1f, 100.0f);
		}

		/*
			fov - FOV in degrees
			ratio - Aspect ratio
			near - Z Near
			far - Z Far
		*/
		void setProjectionMatrix(float fov, float ratio, float near, float far) {			
			projection_matrix = glm::perspective(glm::radians(fov), ratio, near, far);
		}

		glm::mat4 getProjectionMatrix() {
			return projection_matrix;
		}
};


