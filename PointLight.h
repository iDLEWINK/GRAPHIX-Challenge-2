#include "Light.h"
#pragma once

class PointLight : public Light
{
	public:
		glm::vec3 lightPos;

		float constant;
		float linear;
		float quadratic;

		PointLight(glm::vec3 lightPos, glm::vec3 lightColor, float lightStr, glm::vec3 ambientColor, float ambientStr, glm::vec3 specColor, float specStr, float specPhong)
			: Light(lightColor, lightStr, ambientColor, ambientStr, specColor, specStr, specPhong) 
		{
			this->lightPos = lightPos;
			/* DEFAULT RECOMMENDED VALUES FOR ATTENUATION; UNLIKELY TO CHANGE */
			this->constant = 1.0f;		// Constant value for attenuation
			this->linear = 0.35f;		// Linear value for attenuation
			this->quadratic = 0.44f;	// Quadratic value for attenuation
		}
};

