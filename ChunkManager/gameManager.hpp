#pragma once
#include "chunk.hpp"
#include "Player/player.hpp"
#include <vector>
#include "Blocks.hpp"
#include "UI/HUD.hpp"

struct BlockSide {
	bool found;
	int x;
	int y;
	int z;
	Blocks block;
	Faces side;
};

class gameManager {
public:
	Player player;

	std::vector<Chunk> chunks;

	void* window;

	int width = 1024;
	int height = 768;

	float deltaTime;
	double lastTime;

	HUDManager HUD;

	gameManager();
	void m_key_callback(void* window, int key, int scancode, int action, int mods);
	void m_mouse_callback(void* window, int button, int action, int mods);
	void begin();
	BlockSide raycast(glm::vec3 origin, glm::vec3 direction);

	void placeBlock(int x, int y, int z, Blocks block);

	// Finds a loaded chunk that contains the given block
	// Sets chunkIndex to -1 if the chunk isn't found
	void findChunk(int x, int y, int z, int& chunkIndex);
	// Finds a loaded chunk that contains the given block
	void findChunk(int x, int y, int z, int& chunkIndex, bool& chunkFound);
	~gameManager();
};