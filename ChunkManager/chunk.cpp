#include "chunk.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>
#include <iostream>

#include "../Textures/bmp.hpp"

Chunk::Chunk(int x, int z) {
	posX = x;
	posZ = z;
	blocks = std::vector<std::vector<std::vector<Blocks>>>(CHUNK_WIDTH, std::vector<std::vector<Blocks>>(CHUNK_DEPTH, std::vector<Blocks>(CHUNK_HEIGHT, air)));
	for (int x = 0; x < blocks.size(); x++) {
		for (int z = 0; z < blocks[x].size(); z++) {
			blocks[x][0][z] = grass;
		}
	}
	blocks[0][0][0] = dirt;
	vertex_buffer_data = {
	};
	index_buffer_data = {
	};
	uv_buffer_data = {
	};

	Model = glm::translate(glm::mat4(1.0f), glm::vec3(x * 16, 0.0f, z * 16));
	//Model = glm::mat4(1.0f);
}

Chunk::~Chunk() {

}

enum Faces {Top = 0, Bottom, Front, Back, Left, Right};

static int atlasXsize = 2;
static int atlasYsize = 2;
void Chunk::getUVofTexture(int index, float& u1, float& u2, float& v1, float& v2) {
	int Xindex = index % atlasXsize;
	int Yindex = (index - Xindex) / atlasXsize;
	float textureXWidth = 1.0f / atlasXsize;
	float textureYWidth = 1.0f / atlasYsize;
	u1 = Xindex * textureXWidth;
	v1 = Yindex * textureYWidth;
	u2 = (Xindex + 1) * textureXWidth;
	v2 = (Yindex + 1) * textureYWidth;
}

void Chunk::push_vertex(float x, float y, float z) {
	vertex_buffer_data.push_back(x);
	vertex_buffer_data.push_back(y);
	vertex_buffer_data.push_back(z);
}

void Chunk::push_index(unsigned int offset, unsigned int a, unsigned int b, unsigned int c) {
	index_buffer_data.push_back(a + offset);
	index_buffer_data.push_back(b + offset);
	index_buffer_data.push_back(c + offset);
}

void Chunk::push_uv(float u, float v) {
	uv_buffer_data.push_back(u);
	uv_buffer_data.push_back(v);
}

// TODO: simplify
// TODO: fix texture orientation
// TODO: allow texture rotation
void Chunk::GenerateFace(float x, float y, float z, float face, int texture) {
	int vertexBase = (int)vertex_buffer_data.size() / 3;

	float u1, u2, v1, v2;

	getUVofTexture(texture, u1, u2, v1, v2);

	if (face == Top) {
		y++;
		push_vertex(x + 0, y + 0, z + 0);
		push_vertex(x + 0, y + 0, z + 1);
		push_vertex(x + 1, y + 0, z + 1);
		push_vertex(x + 1, y + 0, z + 0);
	}
	else if (face == Bottom) {
		push_vertex(x + 0, y + 0, z + 0);
		push_vertex(x + 1, y + 0, z + 0);
		push_vertex(x + 1, y + 0, z + 1);
		push_vertex(x + 0, y + 0, z + 1);
	}
	else if (face == Front) {
		x++;
		push_vertex(x + 0, y + 0, z + 0);
		push_vertex(x + 0, y + 1, z + 0);
		push_vertex(x + 0, y + 1, z + 1);
		push_vertex(x + 0, y + 0, z + 1);
	}
	else if (face == Back) {
		push_vertex(x + 0, y + 0, z + 0);
		push_vertex(x + 0, y + 0, z + 1);
		push_vertex(x + 0, y + 1, z + 1);
		push_vertex(x + 0, y + 1, z + 0);
	}
	else if (face == Left) {
		push_vertex(x + 0, y + 0, z + 0);
		push_vertex(x + 0, y + 1, z + 0);
		push_vertex(x + 1, y + 1, z + 0);
		push_vertex(x + 1, y + 0, z + 0);
	}
	else if (face == Right) {
		z++;
		push_vertex(x + 0, y + 0, z + 0);
		push_vertex(x + 1, y + 0, z + 0);
		push_vertex(x + 1, y + 1, z + 0);
		push_vertex(x + 0, y + 1, z + 0);
	}
	push_index(vertexBase, 0, 1, 2);
	push_index(vertexBase, 0, 2, 3);

	push_uv(u2, v1);
	push_uv(u2, v2);
	push_uv(u1, v2);
	push_uv(u1, v1);
}

void Chunk::GenerateModel() {
	for (int x = 0; x < blocks.size(); x++) {
		for (int y = 0; y < blocks[x].size(); y++) {
			for (int z = 0; z < blocks[x][y].size(); z++) {
				if (data[blocks[x][y][z]].visible) {
					GenerateFace((float)x, (float)y, (float)z, Top, data[blocks[x][y][z]].topTexIndex);
					GenerateFace((float)x, (float)y, (float)z, Bottom, data[blocks[x][y][z]].bottomTexIndex);
					GenerateFace((float)x, (float)y, (float)z, Front, data[blocks[x][y][z]].frontTexIndex);
					GenerateFace((float)x, (float)y, (float)z, Back, data[blocks[x][y][z]].backTexIndex);
					GenerateFace((float)x, (float)y, (float)z, Left, data[blocks[x][y][z]].leftTexIndex);
					GenerateFace((float)x, (float)y, (float)z, Right, data[blocks[x][y][z]].rightTexIndex);
				}
			}
		}
	}
}

void Chunk::CreateBuffers() {
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uv_buffer_data.size(), uv_buffer_data.data(), GL_STATIC_DRAW);

	texture = loadBMP_custom("C:/Users/ben_l/source/repos/Blockmine/Textures/texture_atlas.bmp");
}

void Chunk::Draw(unsigned int programID, unsigned int MatrixID, glm::mat4 Projection, glm::mat4 View) {
	glm::mat4 mvp = Projection * View * Model;
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,
		3,
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