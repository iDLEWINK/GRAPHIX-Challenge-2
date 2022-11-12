#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma once

class MyCamera
{	private:
		glm::vec3 cameraPos, cameraCenter, WorldUp;
		glm::mat4 viewMatrix, projectionMatrix;
		float yaw, pitch;
		
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
		MyCamera(glm::vec3 cameraPos, glm::vec3 cameraCenter, glm::vec3 WorldUp, float yaw, float pitch) {
			this->cameraPos = cameraPos;
			this->cameraCenter = cameraCenter;
			this->WorldUp = WorldUp;
			this->yaw = yaw;
			this->pitch = pitch;						

			updateViewMatrix();
		}

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

		/* PITCH */
		float getPitch() {
			return pitch;
		}

		void setPitch(float pitch) {
			this->pitch = pitch;
		}

		/* YAW */
		float getYaw() {
			return yaw;
		}

		void setYaw(float yaw) {
			this->yaw = yaw;
		}		

		/* CAMERA POS */
		void setCameraPos(glm::vec3 cameraPos) {
			this->cameraPos = cameraPos;
		}

		glm::vec3 getCameraPos() {
			return cameraPos;
		}

		glm::mat4 getViewMatrix() {
			updateViewMatrix();
			return viewMatrix;
		}

		void setProjectionMatrix(glm::mat4 projectionMatrix) {
			this->projectionMatrix = projectionMatrix;
		}

		glm::mat4 getProjectionMatrix() {
			return projectionMatrix;
		}
};

