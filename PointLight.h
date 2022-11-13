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
			/* 
				NOTE: Values are determined from a recommended light combinations table from Ogre3D - www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
				float constant - Ensures the denominator never gets smaller than 1
				float linear - Reduces the intensity in in a linear fashion
				float quadratic - Quadratic decrease of light source intensity
			*/
			this->constant = 1.0f;		// Constant value for attenuation
			this->linear = 0.35f;		// Linear value for attenuation
			this->quadratic = 0.44f;	// Quadratic value for attenuation
		}
};

