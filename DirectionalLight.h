#include "Light.h"
#pragma once

/* Directional light derived class by inheriting base class Light */
class DirectionalLight : public Light
{
	public:
		glm::vec3 direction;
		/* Directional light constructor */
		DirectionalLight(glm::vec3 direction, glm::vec3 lightColor, float lightStr, glm::vec3 ambientColor, float ambientStr, glm::vec3 specColor, float specStr, float specPhong)
			: Light(lightColor, lightStr, ambientColor, ambientStr, specColor, specStr, specPhong) 
		{
			/* Assign value to directional light direction attribute */
			this->direction = direction;
		}
};

