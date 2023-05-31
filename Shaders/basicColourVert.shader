#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColour;

out vec3 fragmentColour;

//uniform mat4 MVP;

void main() {
	gl_Position = vec4(vertexPosition_modelspace, 1);

	fragmentColour = vertexColour;
}