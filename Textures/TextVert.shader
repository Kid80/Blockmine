#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_screenspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexFG;
layout(location = 3) in vec4 vertexBG;

uniform int screenWidth;
uniform int screenHeight;
uniform int scale;
// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec4 fg;
out vec4 bg;

void main() {
	float sW = screenWidth;
	float sH = screenHeight;

	float cW = 32 / sW * scale;
	float cH = 32 / sH * scale;

	gl_Position = vec4(vertexPosition_screenspace.x * cW - 1, -(vertexPosition_screenspace.y * cH - 1), 0.5, 1);

	// UV of the vertex. No special space for this one.
	UV = vertexUV;
	fg = vertexFG;
	bg = vertexBG;
}