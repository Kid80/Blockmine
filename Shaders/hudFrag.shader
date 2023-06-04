#version 330 core

in vec4 fragmentColour;

out vec4 color;

//uniform sampler2D myTextureSampler;

void main() {
	color = fragmentColour;
	//color = vec4(1.0, 0.0, 0.0, 1.0);
}