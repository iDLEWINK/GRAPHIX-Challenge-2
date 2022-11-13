#version 330 core // Shader version

/* FINAL FRAG COLOR */
out vec4 FragColor;
/* RGBA UNIFORM VARIABLE FOR MANIPULATION */
uniform vec4 color_rgba;

void main(){
	/* Set the final frag color to the value of the color_rgba uniform variable */
	FragColor = color_rgba;
}