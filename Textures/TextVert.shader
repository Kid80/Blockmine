#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_screenspace;
layout(location = 1) in vec2 vertexUV;

uniform mat4 MVP;
// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main() {
	//gl_Position = MVP * vec4(vertexPosition_screenspace.xy, 1, 1);
	gl_Position = vec4(vertexPosition_screenspace.xy, 0.5, 1);

	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}