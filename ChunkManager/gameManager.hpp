#pragma once
#include "chunk.hpp"
#include "Player/player.hpp"
#include <vector>

class worldManager {
public:
	Player player;

	std::vector<Chunk> chunks;

	void* window;

	int width = 1024;
	int height = 1024;

	float deltaTime;
	double lastTime;

	worldManager();
	void begin();
	~worldManager();
};