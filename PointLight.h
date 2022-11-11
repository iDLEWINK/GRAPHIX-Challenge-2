#include "Light.h"
#pragma once

class PointLight : public Light
{
	public:
		glm::vec3 lightPos;

		float constant;
		float linear;
		float quadratic;

		PointLight(glm::vec3 lightPos, glm::vec3 lightColor, float lightStr, glm::vec3 ambientColor, float ambientStr, glm::vec3 specColor, float specStr, float specPhong, float constant, float linear, float quadratic)
			: Light(lightColor, lightStr, ambientColor, ambientStr, specColor, specStr, specPhong) 
		{
			this->lightPos = lightPos;
			this->constant = constant;
			this->linear = linear;
			this->quadratic = quadratic;
		}
};

