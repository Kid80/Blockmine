#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.hpp"
#include "../stb/stb_image.h"

// Loads a bitmap into a texture
// Throws 1 if image not found
unsigned int loadBMP_custom(const char* imagepath) {
	int width, height, nrChannels;
	unsigned char* data = stbi_load(imagepath, &width, &height, &nrChannels, 0);

	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	if (!data) {
		printf("Failed to load texture\n");
		//while (1);
		stbi_image_free(data);
		throw 1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

// Loads bitmap data
// Throws 1 if image not found
unsigned char* loadBMP(const char* imagepath, int* image_width, int* image_height) {
	int nrChannels;
	unsigned char* data = stbi_load(imagepath, image_width, image_height, &nrChannels, 0);
	if (!data) {
		throw 1;
	}
}

// Creates a texture from image data
// Throws 1 if data not found
void LoadTexture(unsigned int* out_texture, int image_width, int image_height, unsigned char* image_data) {
	if (image_data == nullptr) {
		*out_texture = 0;
		throw 1;
		return;
	}
	unsigned int image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	*out_texture = image_texture;

	glBindTexture(GL_TEXTURE_2D, 0);

	return;
}

void updateTexture(unsigned int* texture_ptr, int width, int height, unsigned char* data) {
	glBindTexture(GL_TEXTURE_2D, *texture_ptr);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}