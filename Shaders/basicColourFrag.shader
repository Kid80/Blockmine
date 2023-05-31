#version 330 core

in vec3 fragmentColour;

out vec3 color;

//uniform sampler2D myTextureSampler;

void main() {
	color = fragmentColour;
}