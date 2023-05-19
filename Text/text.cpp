#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "text.hpp"
#include "../Textures/bmp.hpp"
#include "../Shaders/shaders.hpp"
#include <iostream>

TextManager::TextManager() {
	text = {
		text_t{0, (char*)"A", -0.5f, -0.5f}
	};
	std::cout << "init\n";
	vertex_buffer_data.push_back(0.0f);
	vertex_buffer_data.push_back(0.0f);

	vertex_buffer_data.push_back(1.0f);
	vertex_buffer_data.push_back(0.0f);

	vertex_buffer_data.push_back(1.0f);
	vertex_buffer_data.push_back(1.0f);

	vertex_buffer_data.push_back(0.0f);
	vertex_buffer_data.push_back(1.0f);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	texture_buffer_data.push_back(0.0f);
	texture_buffer_data.push_back(0.0f);

	texture_buffer_data.push_back(1.0f);
	texture_buffer_data.push_back(0.0f);

	texture_buffer_data.push_back(1.0f);
	texture_buffer_data.push_back(1.0f);

	texture_buffer_data.push_back(0.0f);
	texture_buffer_data.push_back(1.0f);
}

void TextManager::CreateBuffers() {
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_buffer_data.size(), vertex_buffer_data.data(), GL_STATIC_DRAW);
	
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_buffer_data.size(), index_buffer_data.data(), GL_STATIC_DRAW);
	
	glGenBuffers(1, &texturebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texture_buffer_data.size(), texture_buffer_data.data(), GL_STATIC_DRAW);
	
	programID = LoadShaders("C:/Users/ben_l/source/repos/Blockmine/Textures/TextVert.shader", "C:/Users/ben_l/source/repos/Blockmine/Textures/TextFrag.shader");

	Texture = loadBMP_custom("C:/Users/ben_l/source/repos/Blockmine/Text/Font.bmp");
}

void TextManager::

void TextManager::Draw() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	glUseProgram(programID);
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	glm::mat4 mvp = glm::mat4(1.0f);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

	glDrawElements(
		GL_TRIANGLES,
		vertex_buffer_data.size(),
		GL_UNSIGNED_INT,
		(void*)0
	);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_data.size() / 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}