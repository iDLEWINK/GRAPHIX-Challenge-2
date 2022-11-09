#version 330 core // Version number

layout(location = 0) in vec3 aPos; 

uniform mat4 transform; 
uniform mat4 projection;
uniform mat4 view;

void main(){
	gl_Position = projection * view * transform * vec4(aPos, 1.0); // Combines all coordinate matrices to a final one
}