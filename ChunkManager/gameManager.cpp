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
	///__debugbreak();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	worldManager* worldmanager = (worldManager*)glfwGetWindowUserPointer(window);
	worldmanager->m_key_callback(window, key, scancode, action, mods);
}

worldManager::worldManager() {

}

void worldManager::m_key_callback(void* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		if (glfwGetInputMode((GLFWwindow*)window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
			glfwSetInputMode((GLFWwindow*)window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode((GLFWwindow*)window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

	glfwSetKeyCallback((GLFWwindow*)window, key_callback);

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

	Chunk chunk(0, 0);
	Chunk chunk2(1, 0);

	chunk.GenerateModel();
	chunk.GenerateBuffers();

	chunk2.GenerateModel();
	chunk2.GenerateBuffers();


	GLuint programID = LoadShaders("C:/Users/ben_l/source/repos/Blockmine/Shaders/vert.shader", "C:/Users/ben_l/source/repos/Blockmine/Shaders/frag.shader");

	GLuint matrixID = glGetUniformLocation(programID, "MVP");

	GLuint texture = loadBMP_custom("C:/Users/ben_l/source/repos/Blockmine/Textures/texture_atlas.bmp");

	lastTime = glfwGetTime();
	do {
		glfwGetWindowSize((GLFWwindow*)window, &width, &height);
		glViewport(0, 0, width, height);
		double currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;

		player.update(deltaTime);

		glm::mat4 Projection = player.getProjMat();
		glm::mat4 View = player.getViewMat();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, texture);

		chunk.Draw(programID, matrixID, Projection, View);
		chunk2.Draw(programID, matrixID, Projection, View);

		if (deltaTime * 1000 >= 100) {
			// std::cout << "Delta time: " << deltaTime * 1000 << "ms" << std::endl;
		}

		glfwSwapBuffers((GLFWwindow*)window);
		glfwPollEvents();
	} while (glfwWindowShouldClose((GLFWwindow*)window) == 0);

	glfwTerminate();
}


worldManager::~worldManager() {

}