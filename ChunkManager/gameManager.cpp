#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gameManager.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shaders/shaders.hpp"
#include "Textures/bmp.hpp"
#include "Player/player.hpp"
#include "ChunkManager/chunk.hpp"
#include "Text/text.hpp"

void error_callback(int error, const char* description) {
	printf("Error: %s\n", description);
	//__debugbreak();
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
	if (type == GL_DEBUG_TYPE_ERROR) {
		//__debugbreak();
	}
	///__debugbreak();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	gameManager* worldmanager = (gameManager*)glfwGetWindowUserPointer(window);
	worldmanager->m_key_callback(window, key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	gameManager* worldmanager = (gameManager*)glfwGetWindowUserPointer(window);
	worldmanager->m_mouse_callback(window, button, action, mods);
}

gameManager::gameManager() {

}

void gameManager::m_key_callback(void* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		if (glfwGetInputMode((GLFWwindow*)window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
			glfwSetInputMode((GLFWwindow*)window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode((GLFWwindow*)window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		chunks[0].PlaceBlock(0, 1, 0, dirt);
	}
}

void gameManager::m_mouse_callback(void* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		std::cout << "Right click";
	}
}

void gameManager::begin() {
	glewExperimental = true;
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialise GLFW\n");
		exit(-1);
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback(error_callback);

	window = glfwCreateWindow(width, height, "Blockmine", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		exit(-1);
	}
	glfwSetWindowUserPointer((GLFWwindow*)window, this);
	glfwSetKeyCallback((GLFWwindow*)window, key_callback);
	glfwSetMouseButtonCallback((GLFWwindow*)window, mouse_button_callback);

	glfwMakeContextCurrent((GLFWwindow*)window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialise GLEW\n");
		exit(-1);
	}
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
	//glfwSetInputMode((GLFWwindow*)window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	player.init(window, glm::vec3(0, 2, 0));

	chunks.push_back(Chunk(0, 0, 0));
	chunks.push_back(Chunk(1, 0, 0));

	//Chunk chunk(0, 0);
	//Chunk chunk2(1, 0);

	chunks[0].GenerateModel();
	chunks[1].GenerateModel();

	GLuint programID = LoadShaders("C:/Users/ben_l/source/repos/Blockmine/Shaders/vert.shader", "C:/Users/ben_l/source/repos/Blockmine/Shaders/frag.shader");

	GLuint matrixID = glGetUniformLocation(programID, "MVP");

	lastTime = glfwGetTime();
	do {
		glfwGetWindowSize((GLFWwindow*)window, &width, &height);
		bool windowMinimised = width == 0 && height == 0;
		glViewport(0, 0, width, height);
		double currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!windowMinimised) {
			player.update(deltaTime);

			glm::mat4 Projection = player.getProjMat();
			glm::mat4 View = player.getViewMat();
			//glBindTexture(GL_TEXTURE_2D, texture);
			chunks[0].Draw(programID, matrixID, Projection, View);
			chunks[1].Draw(programID, matrixID, Projection, View);
			//glBindTexture(GL_TEXTURE_2D, 0);
		}
		if (deltaTime * 1000 >= 100) {
			std::cout << "Delta time: " << deltaTime * 1000 << "ms" << std::endl;
		}
		glfwSwapBuffers((GLFWwindow*)window);
		glfwPollEvents();
	} while (glfwWindowShouldClose((GLFWwindow*)window) == 0);
}

bool signedVolume(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
	using namespace glm;
	return std::signbit((1.0 / 6.0) * dot(cross(b - a, c - a), d - a));
}

//If SignedVolume(q1, p1, p2, p3) and SignedVolume(q2, p1, p2, p3) have different signs AND 
//SignedVolume(q1, q2, p1, p2), SignedVolume(q1, q2, p2, p3) and SignedVolume(q1, q2, p3, p1) have the same sign, then there is an intersection.

// This is a really bad algorithm but it's simple
Blocks gameManager::raycast(glm::vec3 origin, glm::vec3 direction) {
	direction = glm::normalize(direction);
	float minDistance = 10;
	bool xSign = std::signbit(direction.x);
	bool ySign = std::signbit(direction.y);
	bool zSign = std::signbit(direction.z);
	glm::vec3 roundedPosition = glm::round(origin);

	bool blockFound = false;
	BlockSide block = { 0, 0, 0, air, Top };

	for (int z = roundedPosition.z - 11; z < roundedPosition.z + 11; z++) {
		int chunkZ = (z - (z % 16)) / 16;
		for (int y = roundedPosition.y - 11; y < roundedPosition.y + 11; y++) {
			int chunkY = (y - (y % 16)) / 16;
			for (int x = roundedPosition.x - 11; x < roundedPosition.x + 11; x++) {
				int chunkX = (x - (x % 16)) / 16;
				int chunkIndex = 0;
				bool chunkFound = false;
				for (int i = 0; i < chunks.size(); i++) {
					if (chunks[i].posX == chunkX && chunks[i].posY == chunkY && chunks[i].posZ == chunkZ) {
						chunkIndex = i;
						chunkFound = true;
					}
				}
				if (chunkFound) {
					if (data[chunks[chunkIndex].blocks[x][y][z]].visible) {
						if (xSign) {

						}
					}
				}
			}
		}
	}
}


gameManager::~gameManager() {

	glfwTerminate();
}