#include "Light.h"
#pragma once

class DirectionalLight : public Light
{
	public:
		glm::vec3 direction;

		DirectionalLight(glm::vec3 direction, glm::vec3 lightColor, glm::vec3 ambientColor, float ambientStr, glm::vec3 specColor, float specStr, float specPhong)
			: Light(lightColor, ambientColor, ambientStr, specColor, specStr, specPhong) 
		{
			this->direction = direction;
		}
};

