#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma once

/* Base Light Class */
class Light
{
	/* Attributes are public for easy access and value manipulation */
	public:
		glm::vec3 lightColor, ambientColor, specColor;
		float lightStr, ambientStr, specStr, specPhong;

		/* Constructor - accepts the necessary light attributes; NOTE - vec3 pos/direction delecated to the derived classes to prevent ambiguity with regards to context */
		Light(glm::vec3 lightColor, float lightStr, glm::vec3 ambientColor, float ambientStr, glm::vec3 specColor, float specStr, float specPhong) {
			this->lightColor = lightColor;
			this->lightStr = lightStr;
			this->ambientColor = ambientColor;
			this->ambientStr = ambientStr;
			this->specColor = specColor;
			this->specStr = specStr;
			this->specPhong = specPhong;
		}		
};

