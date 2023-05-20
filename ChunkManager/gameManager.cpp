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

Player player;
float movementKeys[2];

int width = 1024;
int height = 768;
float deltaTime;
double lastTime;

GLFWwindow* window;

double cursorXpos;
double cursorYpos;
double cursorXposOld;
double cursorYposOld;
double cursorXdelta;
double cursorYdelta;

void error_callback(int error, const char* description) {
	printf("Error: %s\n", description);
	//__debugbreak();
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
	///__debugbreak();
}

void updateCamera() {
	glfwGetCursorPos(window, &cursorXpos, &cursorYpos);
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		cursorXdelta = cursorXpos - cursorXposOld;
		cursorYdelta = cursorYpos - cursorYposOld;
	}
	else {
		cursorXdelta = 0.0;
		cursorYdelta = 0.0;
	}
	cursorXposOld = cursorXpos;
	cursorYposOld = cursorYpos;

	float kbdX = 0.0f;
	kbdX += glfwGetKey(window, GLFW_KEY_W);
	kbdX -= glfwGetKey(window, GLFW_KEY_S);
	float kbdY = 0.0f;
	kbdY -= glfwGetKey(window, GLFW_KEY_A);
	kbdY += glfwGetKey(window, GLFW_KEY_D);
	float kbdZ = 0.0f;
	kbdZ += glfwGetKey(window, GLFW_KEY_Q);
	kbdZ -= glfwGetKey(window, GLFW_KEY_E);
	player.horizontalAngle -= 0.5f * deltaTime * (float)cursorXdelta;
	player.verticalAngle -= 0.5f * deltaTime * (float)cursorYdelta;

	//std::cout << "X: " << player.position.x << " Y: " << player.position.y << " Z: " << player.position.z << std::endl;
	player.getDirections();
	float speed = 5.0f;
	player.move(
		kbdX * deltaTime * speed,
		kbdY * deltaTime * speed,
		kbdZ * deltaTime * speed
	);
	player.width = width;
	player.height = height;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}



void worldManager::begin() {
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

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialise GLEW\n");
		exit(-1);
	}
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	player = Player(window);
	player.position = glm::vec3(0, 2, 0);

	Chunk chunk(0, 0);
	Chunk chunk2(1, 0);

	chunk.GenerateModel();
	chunk.CreateBuffers();

	chunk2.GenerateModel();
	chunk2.CreateBuffers();


	GLuint programID = LoadShaders("C:/Users/ben_l/source/repos/Blockmine/Shaders/vert.shader", "C:/Users/ben_l/source/repos/Blockmine/Shaders/frag.shader");

	GLuint matrixID = glGetUniformLocation(programID, "MVP");

	GLuint texture = loadBMP_custom("C:/Users/ben_l/source/repos/Blockmine/Textures/texture_atlas.bmp");

	lastTime = glfwGetTime();
	do {
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		double currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;

		updateCamera();

		glm::mat4 Projection = player.getProjMat();
		glm::mat4 View = player.getViewMat();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, texture);

		chunk.Draw(programID, matrixID, Projection, View);
		chunk2.Draw(programID, matrixID, Projection, View);

		if (deltaTime * 1000 >= 100) {
			// std::cout << "Delta time: " << deltaTime * 1000 << "ms" << std::endl;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwWindowShouldClose(window) == 0);

	glfwTerminate();
}


worldManager::~worldManager() {

}