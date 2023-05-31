#pragma once
#include "chunk.hpp"
#include "Player/player.hpp"
#include <vector>
#include "Blocks.hpp"

struct BlockSide {
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


	gameManager();
	void m_key_callback(void* window, int key, int scancode, int action, int mods);
	void m_mouse_callback(void* window, int button, int action, int mods);
	void begin();
	Blocks raycast(glm::vec3 origin, glm::vec3 direction);
	~gameManager();
};