#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColour;

out vec4 fragmentColour;

//uniform mat4 MVP;
uniform int screenWidth;
uniform int screenHeight;
uniform float scale;

void main() {
	float sW = screenWidth;
	float sH = screenHeight;

	float cW = 2 / sW * scale;
	float cH = 2 / sH * scale;

	gl_Position = vec4(vertexPosition_modelspace.x * cW, vertexPosition_modelspace.y * cH, vertexPosition_modelspace.z, 1);

	fragmentColour = vec4(vertexColour, 1);
}