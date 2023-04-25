#pragma once
#include "chunk.h"
#include "../Camera/camera.h"
#include <vector>

class worldManager {
public:
	std::vector<Chunk> chunks;

	worldManager();

	void Load();
	void Draw(unsigned int programID, unsigned int MatrixID, glm::mat4 projection, glm::mat4 view);
};