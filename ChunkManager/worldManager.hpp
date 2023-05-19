#pragma once
#include "chunk.hpp"
#include "../Camera/camera.hpp"
#include <vector>

class worldManager {
public:
	std::vector<Chunk> chunks;

	worldManager();

	void Load();
	void Draw(unsigned int programID, unsigned int MatrixID, glm::mat4 projection, glm::mat4 view);
};