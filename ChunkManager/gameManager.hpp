#pragma once
#include "chunk.hpp"
#include "../Camera/camera.hpp"
#include <vector>

class worldManager {
public:
	std::vector<Chunk> chunks;

	worldManager();
	void begin();
	~worldManager();
};