/* POINT LIGHT */

#version 330 core //shader version

uniform sampler2D tex0;

uniform vec3 pointLightPos; // light source
uniform vec3 pointLightColor; // color of the light source
uniform float pointLightStr; // intensity of light source

uniform float pointAmbientStr; // ambient intensity or strength
uniform vec3 pointAmbientColor; // color of the ambient or reflected light

uniform vec3 cameraPos;
uniform vec3 pointSpecColor; // specular light color
uniform float pointSpecStr; // specular intensity or strength
uniform float pointSpecPhong; // spread or concentration of the specular light

// Receive the output values from vertex shader
in vec2 texCoord; 
in vec3 normCoord;
in vec3 fragPos;

out vec4 FragColor;

void main(){

    vec3 normal = normalize(normCoord);




    /* POINT LIGHT CALCULATION */   
    vec3 pointLightDir = normalize(pointLightPos - fragPos); // Direction from the light source to your fragment source

    /* DIFFUSE */
    float pointDiff = max(dot(normal, pointLightDir), 0.0f); // Max so we do not have any negative lights
    vec3 pointDiffuse = pointDiff * pointLightStr * pointLightColor; // Multiply diffuse light to light color (and even intensity)

    /* AMBIENT */
    vec3 pointAmbientCol = pointAmbientStr * pointAmbientColor; // NOTE: pointAmbientCol != pointAmbientColor; pointAmbientCol is the final ambient

    /* SPECULAR */
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-pointLightDir, normal); // Reflection vector    
    float spec = pow(max(dot(reflectDir, viewDir), 0.1f), pointSpecPhong); // Specular light
    vec3 specCol = spec * pointSpecStr * pointSpecColor; //  Combine spec with intensity and chosen RGB coler

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
    specCol = specCol * attenuation;        // transform the specular vector with respect to the attenuation









    // Fix this later with directional
    FragColor = vec4(specCol + pointDiffuse + pointAmbientCol, 1.0f) * texture(tex0, texCoord); // Assign the pixels, given our UV, to the model of our object; The wrapping part
                                                                // Apply the diffuse
                                                                // Apply the ambient
                                                                // Apply specular    
}