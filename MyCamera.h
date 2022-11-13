#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma once

/* Base Camera Class */
class MyCamera
{	private:
		glm::vec3 cameraPos, cameraCenter, WorldUp;
		glm::mat4 viewMatrix, projectionMatrix;
		float yaw, pitch;
		
		/* Compute for the view matrix and assign to the private attribute */
		void updateViewMatrix() {
			glm::mat4 cameraPosMatrix = glm::translate(glm::mat4(1.0f), cameraPos * -1.0f);

			/* Three camera vectors */
			glm::vec3 F = cameraCenter - cameraPos;
			F = glm::normalize(F);
			glm::vec3 R = glm::cross(F, WorldUp); 
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
		/* Constructor - takes camera position, camera center, and world up */
		MyCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 WorldUp) {
			this->cameraPos = cameraPos;
			this->cameraCenter = cameraCenter;
			this->WorldUp = WorldUp;
			this->yaw = 90.0f;		// Default value for yaw
			this->pitch = 0.0f;		// Default value for pitch					

			/* Immediately compute for the view matrix and save it */
			updateViewMatrix();
		}

		/* Updates the camera position with respect to a multiplier (distance) */
		void updateCameraPos(float multiplier) {
			/*
				Set the updated direction values (Pythagorean Theorem)
				yaw - x-axis (cos) and z-axis (sin)
				pitch - x-axis (cos), z-axis (cos), y-axis(sin)
			*/
			cameraPos.x = multiplier * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraPos.y = multiplier * sin(glm::radians(pitch));
			cameraPos.z = multiplier * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		}

		/* Pitch getter and setter */
		float getPitch() {
			return pitch;
		}

		void setPitch(float pitch) {
			this->pitch = pitch;
		}

		/* Yaw getter and setter */
		float getYaw() {
			return yaw;
		}

		void setYaw(float yaw) {
			this->yaw = yaw;
		}		

		/* Camera position getter and setter */
		glm::vec3 getCameraPos() {
			return cameraPos;
		}

		void setCameraPos(glm::vec3 cameraPos) {
			this->cameraPos = cameraPos;
		}		
		
		/* Camera center getter and setter */
		glm::vec3 getCameraCenter() {
			return cameraCenter;
		}

		void setCameraCenter(glm::vec3 cameraCenter) {
			this->cameraCenter = cameraCenter;
		}

		/* World Up getter and setter */
		glm::vec3 getWorldUp() {
			return WorldUp;
		}

		void setWorldUp(glm::vec3 WorldUp) {
			this->WorldUp = WorldUp;
		}

		/* Projection matrix getter and setter */
		glm::mat4 getProjectionMatrix() {
			return projectionMatrix;
		}

		void setProjectionMatrix(glm::mat4 projectionMatrix) {
			this->projectionMatrix = projectionMatrix;
		}

		/* View matrix getter */
		glm::mat4 getViewMatrix() {
			/* Compute and update the view matrix just in case of new values */
			updateViewMatrix();
			return viewMatrix;
		}

};

