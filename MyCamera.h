#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma once
class MyCamera
{	private:
		glm::vec3 cameraPos, cameraCenter, WorldUp;
		glm::mat4 viewMatrix;
		float yaw, pitch, sensitivity, lastX, lastY;
		
		void updateViewMatrix() {
			//viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraCenter, WorldUp);
			
			glm::mat4 cameraPosMatrix = glm::translate(glm::mat4(1.0f), cameraPos * -1.0f);

			// Three camera vectors 
			glm::vec3 F = cameraCenter - cameraPos;
			F = glm::normalize(F);
			glm::vec3 R = glm::cross(F, WorldUp); // Normalized already so we don't need to normalize anymore. But we can normalize to it again to make sure.
			R = glm::normalize(R);
			glm::vec3 U = glm::cross(R, F);
			U = glm::normalize(U);

			glm::mat4 cameraOrientationMatrix = glm::mat4(1.0f); // Double-sided array

			cameraOrientationMatrix[0][0] = R.x;
			cameraOrientationMatrix[1][0] = R.y;
			cameraOrientationMatrix[2][0] = R.z;

			cameraOrientationMatrix[0][1] = U.x;
			cameraOrientationMatrix[1][1] = U.y;
			cameraOrientationMatrix[2][1] = U.z;

			cameraOrientationMatrix[0][2] = -F.x;
			cameraOrientationMatrix[1][2] = -F.y;
			cameraOrientationMatrix[2][2] = -F.z;

			viewMatrix = cameraOrientationMatrix * cameraPosMatrix;			
		}		

	public:
		MyCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 WorldUp, float yaw, float pitch, float sensitivity, float screenWidth, float screenHeight) {
			this->cameraPos = cameraPos;
			this->cameraCenter = cameraCenter;
			this->WorldUp = WorldUp;
			this->yaw = yaw;
			this->pitch = pitch;
			this->sensitivity = sensitivity;
			lastX = screenWidth / 2.0;
			lastY = screenHeight / 2.0;

			updateViewMatrix();
		}

		void updateCameraDirection() {
			/*
				Set the updated direction values (Pythagorean Theorem)
				yaw - x-axis (cos) and z-axis (sin)
				pitch - x-axis (cos), z-axis (cos), y-axis(sin)
			*/
			glm::vec3 direction;
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

			/* Assign the new "front" or center for the camera; Normalize cause the direction is supposed to be a unit vector (len = 1) */
			cameraCenter = glm::normalize(direction);
		}

		void setPitch(float pitch) {
			this->pitch = pitch;
		}

		void setYaw(float yaw) {
			this->yaw = yaw;
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

		glm::vec3 getCameraPos() {
			return cameraPos;
		}

		glm::mat4 getViewMatrix() {
			updateViewMatrix();
			return viewMatrix;
		}
};

