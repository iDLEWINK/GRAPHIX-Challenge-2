/* POINT LIGHT */

#version 330 core //shader version

uniform sampler2D tex0;

/* POINT LIGHT */
uniform vec3 pointLightPos; // light source
uniform vec3 pointLightColor; // color of the light source
uniform float pointLightStr; // intensity of light source

uniform float pointAmbientStr; // ambient intensity or strength
uniform vec3 pointAmbientColor; // color of the ambient or reflected light

uniform vec3 cameraPos;
uniform vec3 pointSpecColor; // specular light color
uniform float pointSpecStr; // specular intensity or strength
uniform float pointSpecPhong; // spread or concentration of the specular light



/* DIRECTIONAL LIGHT */
uniform vec3 directionalLightPos; // light source
uniform vec3 directionalLightColor;
uniform float directionLightStr;

uniform float directionalAmbientStr; // ambient intensity or strength
uniform vec3 directionalAmbientColor;

uniform vec3 directionalSpecColor; // specular light color
uniform float directionalSpecStr;
uniform float directionalSpecPhong;



/* VERTEX SHADER OUTPUT VARIABLES */
in vec2 texCoord; 
in vec3 normCoord;
in vec3 fragPos;

/* FINAL FRAG COLOR */
out vec4 FragColor;

void main(){

    FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    /* THE SAME FOR SPECULAR COMPUTATION OF BOTH POINT AND DIRECTIONAL */
    vec3 normal = normalize(normCoord);
    vec3 viewDir = normalize(cameraPos - fragPos);

    







    /********* DIRECTIONAL LIGHT CALCULATION *********/   
	vec3 directionalLightDir = normalize(directionalLightPos - fragPos); // Direction from the light source to your fragment source

	/* DIFFUSE */
	float directionalDiff = max(dot(normal, directionalLightDir), 0.0f); // Max so we do not have any negative lights	
	vec3 directionalDiffuse = directionalDiff * directionLightStr * directionalLightColor; // Multiply diffuse light to light color (and even intensity)

	/* AMBIENT */
	vec3 directionalAmbientCol = directionalAmbientStr * directionalAmbientColor; // NOTE: ambientCol != ambientColor; directionalAmbientCol is the final ambient

	/* SPECULAR */
	vec3 directionalReflectDir = reflect(-directionalLightDir, normal); // Reflection vector	
	float directionalSpec = pow(max(dot(directionalReflectDir, viewDir), 0.1f), directionalSpecPhong); // Specular light
	vec3 directionalSpecCol = directionalSpec * directionalSpecStr * directionalSpecColor; // Or any in light color; you can choose your own rgb as opposed to lightColor; final specular color

    /* ADDING DIRECTIONAL TO FRAG COLOR */
	FragColor += vec4(directionalSpecCol + directionalDiffuse + directionalAmbientCol, 1.0f) * texture(tex0, texCoord); // Assign the pixels, given our UV, to the model of our object; The wrapping part
																// Apply the diffuse
																// Apply specular




    /********* POINT LIGHT CALCULATION *********/   
    vec3 pointLightDir = normalize(pointLightPos - fragPos); // Direction from the light source to your fragment source

    /* DIFFUSE */
    float pointDiff = max(dot(normal, pointLightDir), 0.0f); // Max so we do not have any negative lights
    vec3 pointDiffuse = pointDiff * pointLightStr * pointLightColor; // Multiply diffuse light to light color (and even intensity)

    /* AMBIENT */
    vec3 pointAmbientCol = pointAmbientStr * pointAmbientColor; // NOTE: pointAmbientCol != pointAmbientColor; pointAmbientCol is the final ambient

    /* SPECULAR */
    vec3 pointReflectDir = reflect(-pointLightDir, normal); // Reflection vector    
    float pointSpec = pow(max(dot(pointReflectDir, viewDir), 0.1f), pointSpecPhong); // Specular light
    vec3 pointSpecCol = pointSpec * pointSpecStr * pointSpecColor; //  Combine spec with intensity and chosen RGB coler

    /* POINT */
    /* NOTE: Values are determined from a recommended light combinations table from Ogre3D - www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation */
    float constant = 1.0f; // Ensures the denominator never gets smaller than 1
    float linear = 0.0014f; // Reduces the intensity in in a linear fashion
    float quadratic = 0.000007f; // Quadratic decrease of light source intensity

    float distance = length(pointLightPos - fragPos); // solve for the distance between the position of the light source and the object source
    float attenuation = 1.0f / (constant + (linear * distance) + (quadratic * (distance * distance))); // solve for the attenuation via. inverse law; (1.0 / distance ^ 2)

    /* APPLY POINT LIGHT INTENSITY */
    pointDiffuse = pointDiffuse * attenuation;        // transform the diffuse vector with respect to the attenuation
    pointAmbientCol = pointAmbientCol * attenuation;    // transform the ambient vector with respect to the attenuation
    pointSpecCol = pointSpecCol * attenuation;        // transform the specular vector with respect to the attenuation

    /* ADDING POINT TO FRAG COLOR */
    FragColor += vec4(pointSpecCol + pointDiffuse + pointAmbientCol, 1.0f) * texture(tex0, texCoord); // Assign the pixels, given our UV, to the model of our object; The wrapping part
                                                                // Apply the diffuse
                                                                // Apply the ambient
                                                                // Apply specular   
                                                                
    

}