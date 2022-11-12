#version 330 core

out vec4 FragColor;
uniform vec4 color_rgba;

void main(){
	FragColor = color_rgba;
}