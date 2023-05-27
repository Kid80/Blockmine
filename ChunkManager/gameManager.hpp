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
	int height = 768;

	float deltaTime;
	double lastTime;

	worldManager();
	void m_key_callback(void* window, int key, int scancode, int action, int mods);
	void begin();
	~worldManager();
};