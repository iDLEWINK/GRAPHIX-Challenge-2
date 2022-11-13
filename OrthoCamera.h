#include "MyCamera.h"
#pragma once

class OrthoCamera : public MyCamera
{
	public:
		OrthoCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 WorldUp)
			: MyCamera(cameraPos, cameraCenter, WorldUp)
		{			
			/* Set projection_matrix of parent */
			/* Default Settings to initialize the ortographic projection matrix */
			MyCamera::setProjectionMatrix(glm::ortho(-15.0f, 15.0f,
													 -15.0f, 15.0f,
													 -15.0f, 15.0f));
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
			/* Set projection_matrix of parent */
			MyCamera::setProjectionMatrix(glm::ortho(left, right, // L -> R || L < R ; xMin to xMax
													 bottom, top, // B -> T || B < T ; yMin to yMax
													 near, far)); // N -> F || N < F ; zMin to zMax
		}

		glm::mat4 getProjectionMatrix() {
			return MyCamera::getProjectionMatrix();
		}
};

