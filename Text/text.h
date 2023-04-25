#pragma once
#include <vector>

struct text_t {
	int index;
	char* message;
	float posX;
	float posY;
};

class TextManager {
public:
	std::vector<text_t> text;

	std::vector<float> vertex_buffer_data;
	std::vector<unsigned int> index_buffer_data;
	std::vector<float> texture_buffer_data;

	unsigned int VertexArrayID;
	unsigned int vertexbuffer;
	unsigned int elementbuffer;
	unsigned int texturebuffer;
	unsigned int Texture;

	TextManager();
	int printf(char* message, float posX, float posY);
	void clear(int index);
	void CreateBuffers();
	void Draw();
};