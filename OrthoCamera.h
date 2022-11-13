#include "MyCamera.h"
#pragma once

class OrthoCamera : public MyCamera
{
	public:
		OrthoCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 WorldUp)
			: MyCamera(cameraPos, cameraCenter, WorldUp)
		{
			/* Default Settings to initialize the ortographic projection matrix */
			glm::mat4 projection_matrix = glm::ortho(-15.0f, 15.0f,
													-15.0f, 15.0f,
													-15.0f, 15.0f); 
			/* Set projection_matrix of parent */
			MyCamera::setProjectionMatrix(projection_matrix);
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
			glm::mat4 projection_matrix = glm::ortho(left, right, // L -> R || L < R
											bottom, top, // B -> T || B < T
											near, far);
			/* Set projection_matrix of parent */
			MyCamera::setProjectionMatrix(projection_matrix);
		}

		glm::mat4 getProjectionMatrix() {
			return MyCamera::getProjectionMatrix();
		}
};

