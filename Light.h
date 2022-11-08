#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma once

class Light
{
	public:
		glm::vec3 lightColor, ambientColor, specColor;
		float ambientStr, specStr, specPhong;

		Light(glm::vec3 lightColor, glm::vec3 ambientColor, float ambientStr, glm::vec3 specColor, float specStr, float specPhong) {
			this->lightColor = lightColor;
			this->ambientColor = ambientColor;
			this->ambientStr = ambientStr;
			this->specColor = specColor;
			this->specStr = specStr;
			this->specPhong = specPhong;
		}		
};

