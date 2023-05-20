#pragma once
#include "chunk.hpp"
#include "Player/player.hpp"
#include <vector>

class worldManager {
public:
	Player player;

	std::vector<Chunk> chunks;

	worldManager();
	void begin();
	~worldManager();
};