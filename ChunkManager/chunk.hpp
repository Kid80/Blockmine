#pragma once
//#define CHUNK_HEIGHT 16
constexpr int CHUNK_HEIGHT = 16;
constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_DEPTH = 16;

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Blocks.hpp"

class Chunk {
public:
	//blockData* blocksData;
	std::vector<std::vector<std::vector<Blocks>>> blocks;
	int posX, posY, posZ;

	std::vector<float> vertex_buffer_data;
	std::vector<int> index_buffer_data;
	std::vector<float> uv_buffer_data;

	glm::mat4 Model;

	unsigned int VertexArrayID;

	// Array of buffers
	// Index 0 - Positions
	// Index 1 - UVs
	// Index 2 - Elements
	unsigned int buffers[3];
	// unsigned int vertexbuffer;
	// unsigned int texturebuffer;
	// unsigned int elementbuffer;

	unsigned int texture;

	bool buffersGenerated = false;

	void GenerateBlock(int x, int y, int z);
	void getUVofTexture(int index, float& u, float& v, float& u2, float& v2);
	void push_vertex(float x, float y, float z);
	void push_index(unsigned int offset, unsigned int a, unsigned int b, unsigned int c);
	void push_uv(float u, float v);
	void GenerateFace(float x, float y, float z, float face, int texture);
	void GenerateModel();
	void GenerateBuffers();
	void PlaceBlock(int x, int y, int z, Blocks block);
	void Draw(unsigned int programID, unsigned int MatrixID, glm::mat4 projection, glm::mat4 view);

	Chunk(int x, int y, int z);
	~Chunk();
};