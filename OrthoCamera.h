#include "MyCamera.h"
#pragma once

class OrthoCamera : public MyCamera
{
	private:
		glm::mat4 projection_matrix;

	public:
		OrthoCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 WorldUp, float yaw, float pitch)
			: MyCamera(cameraPos, cameraCenter, WorldUp, yaw, pitch)
		{
			/* Default Settings to initialize the ortographic projection matrix; CHANGE THIS TO ARGUMENT ACCEPT */
			projection_matrix = glm::ortho(-1.0f, 1.0f, 
											-1.0f, 1.0f,
											-2.0f, 2.0f); 
		}

		/*
			left - Minimum x
			right - Maximum x
			bottom - Minimum y
			top - Maximum y
			near - Minimum z
			far - Maximum z		
		*/
		void setProjectionMatrix(float left, float right, float bottom, float top, float near, float far) {
			projection_matrix = glm::ortho(left, right, // L -> R || L < R
											bottom, top, // B -> T || B < T
											near, far);
		}

		glm::mat4 getProjectionMatrix() {
			return projection_matrix;
		}
};

