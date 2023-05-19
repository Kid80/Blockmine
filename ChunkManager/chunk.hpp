#pragma once
#define CHUNK_HEIGHT 16
#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Blocks {
	air = 0,
	grass = 1,
	dirt = 2
};

struct blockData {
	bool visible;
	int topTexIndex;
	int bottomTexIndex;
	int frontTexIndex;
	int backTexIndex;
	int leftTexIndex;
	int rightTexIndex;
};

static blockData data[3]{
	{false, 0, 0, 0, 0, 0, 0}, 
	{true, 0, 2, 1, 1, 1, 1},
	{true, 2, 2, 2, 2, 2, 2}
};

class Chunk {
public:
	//blockData* blocksData;
	std::vector<std::vector<std::vector<Blocks>>> blocks;
	int posX, posZ;

	std::vector<float> vertex_buffer_data;
	std::vector<int> index_buffer_data;
	std::vector<float> uv_buffer_data;

	glm::mat4 Model;

	unsigned int VertexArrayID;
	unsigned int vertexbuffer;
	unsigned int texturebuffer;
	unsigned int elementbuffer;

	unsigned int texture;

	void getUVofTexture(int index, float& u, float& v, float& u2, float& v2);
	void push_vertex(float x, float y, float z);
	void push_index(unsigned int offset, unsigned int a, unsigned int b, unsigned int c);
	void push_uv(float u, float v);
	void GenerateFace(float x, float y, float z, float face, int texture);
	void GenerateModel();
	void CreateBuffers();
	void Draw(unsigned int programID, unsigned int MatrixID, glm::mat4 projection, glm::mat4 view);

	Chunk(int x, int z);
	~Chunk();
};