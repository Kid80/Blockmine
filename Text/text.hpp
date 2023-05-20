#pragma once
#include <vector>

struct text_t {
	const char* message;
	float posX;
	float posY;

	unsigned int texture;

	text_t(const char* message, float posX, float posY) {
		this->message = message;
		this->posX = posX;
		this->posY = posY;
		texture = 0;
	}

	~text_t() {

	}
};

class TextManager {
public:
	std::vector<int> textIndices;
	std::vector<text_t> text;

	std::vector<float> vertex_buffer_data;
	std::vector<unsigned int> index_buffer_data;
	std::vector<float> texture_buffer_data;

	unsigned int VertexArrayID;
	unsigned int vertexbuffer;
	unsigned int elementbuffer;
	unsigned int texturebuffer;
	unsigned int Texture;
	unsigned int programID;

	TextManager();
	unsigned int printf(const char* message, float posX, float posY);
	void clear(int index);
	void CreateBuffers();
	void Draw();
};