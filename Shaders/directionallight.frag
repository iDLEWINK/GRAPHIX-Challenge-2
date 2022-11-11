#version 330 core //shader version

uniform sampler2D tex0;

uniform vec3 directionalLightPos; // light source
uniform vec3 directionalLightColor;
uniform float directionLightStr;

uniform float directionalAmbientStr; // ambient intensity or strength
uniform vec3 directionalAmbientColor;

uniform vec3 cameraPos;
uniform vec3 directionalSpecColor; // specular light color
uniform float directionalSpecStr;
uniform float directionalSpecPhong;

in vec2 texCoord; // Receive the output variable for textures in sample.vert
in vec3 normCoord;
in vec3 fragPos;

out vec4 FragColor;

void main(){
	vec3 normal = normalize(normCoord);
	vec3 directionalLightDir = normalize(directionalLightPos - fragPos); // Direction from the light source to your fragment source

	/* DIFFUSE */
	float directionalDiff = max(dot(normal, directionalLightDir), 0.0f); // Max so we do not have any negative lights	
	vec3 directionalDiffuse = directionalDiff * directionLightStr * directionalLightColor; // Multiply diffuse light to light color (and even intensity)

	/* AMBIENT */
	vec3 directionalAmbientCol = directionalAmbientStr * directionalAmbientColor; // NOTE: ambientCol != ambientColor; directionalAmbientCol is the final ambient

	/* SPECULAR */
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 directionalReflectDir = reflect(-directionalLightDir, normal); // Reflection vector	
	float directionalSpec = pow(max(dot(directionalReflectDir, viewDir), 0.1f), directionalSpecPhong); // Specular light
	vec3 directionalSpecCol = directionalSpec * directionalSpecStr * directionalSpecColor; // Or any in light color; you can choose your own rgb as opposed to lightColor; final specular color


	FragColor = vec4(directionalSpecCol + directionalDiffuse + directionalAmbientCol, 1.0f) * texture(tex0, texCoord); // Assign the pixels, given our UV, to the model of our object; The wrapping part
																// Apply the diffuse
																// Apply specular
}