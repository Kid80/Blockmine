#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 fg;
in vec4 bg;

// Ouput data
out vec4 colour;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main() {
	//color = vec4(1, 0, 0, 1);
	//color = texture(myTextureSampler, UV);
	//colour = vec4(1, 0, 0, 1);
	if (texture(myTextureSampler, UV) == vec4(0, 0, 0, 1)) {
		colour = bg;
	}
	else {
		colour = fg;
	}
	//color.a = 0.5;
}