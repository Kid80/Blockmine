#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shaders/shaders.hpp"
#include "Textures/bmp.hpp"
#include "Camera/camera.hpp"
#include "ChunkManager/chunk.hpp"
#include "Text/text.hpp"

Camera camera;
float movementKeys[2];

int width = 1024;
int height = 768;
float deltaTime;
double lastTime;

GLFWwindow* window;

std::vector<float> g_vertex_buffer_data = {
	// bottom
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	// top
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,

	// front
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	// back
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,

	// left
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	// right
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f
};
std::vector<unsigned int> indices = {

	// bottom
	0, 1, 2,
	0, 2, 3,

	// top
	4, 5, 6,
	4, 6, 7,

	// top
	8, 10, 9,
	8, 11, 10,

	// back
	12, 14, 13,
	12, 15, 14,

	// left
	16, 17, 18,
	16, 18, 19,

	// right
	20, 22, 21,
	20, 23, 22
};
std::vector<float> g_uv_buffer_data = {
	
	// bottom
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,

	// top
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	// front
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	// back
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	// left
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	//right
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
};

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
	camera.horizontalAngle -= 0.5f * deltaTime * (float)cursorXdelta;
	camera.verticalAngle -= 0.5f * deltaTime * (float)cursorYdelta;

	//std::cout << "X: " << camera.position.x << " Y: " << camera.position.y << " Z: " << camera.position.z << std::endl;
	camera.getDirections();
	float speed = 5.0f;
	camera.move(
		kbdX * deltaTime * speed,
		kbdY * deltaTime * speed,
		kbdZ * deltaTime * speed
	);
	camera.width = width;
	camera.height = height;
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

// TODO: Refactor
int main() {
	camera = Camera();
	camera.position = glm::vec3(0, 1, 0);
	glewExperimental = true;
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialise GLFW\n");
		return -1;
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
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	Chunk chunk(0, 0);
	Chunk chunk2(1, 0);
	TextManager textManager = TextManager();

	/*GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * g_vertex_buffer_data.size(), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	GLuint texturebuffer;
	glGenBuffers(1, &texturebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * g_uv_buffer_data.size(), g_uv_buffer_data.data(), GL_STATIC_DRAW);*/
	chunk.GenerateModel();
	chunk.CreateBuffers();

	chunk2.GenerateModel();
	chunk2.CreateBuffers();

	textManager.CreateBuffers();



	GLuint programID = LoadShaders("C:/Users/ben_l/source/repos/Blockmine/Shaders/vert.shader", "C:/Users/ben_l/source/repos/Blockmine/Shaders/frag.shader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	GLuint Texture = loadBMP_custom("C:/Users/ben_l/source/repos/Blockmine/Textures/texture_atlas.bmp");
	
	lastTime = glfwGetTime();
	do {
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		//std::cout << "Width: " << width << " Height: " << height << std::endl;
		double currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;

		updateCamera();

		glm::mat4 Projection = camera.getProjMat();

		glm::mat4 View = camera.getViewMat();

		//glm::mat4 Model = glm::mat4(1.0f);

		//glm::mat4 mvp = Projection * View * Model;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, Texture);
		
		chunk.Draw(programID, MatrixID, Projection, View);
		chunk2.Draw(programID, MatrixID, Projection, View);

		textManager.Draw();
		/*glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		glDrawElements(
			GL_TRIANGLES,
			g_vertex_buffer_data.size(),
			GL_UNSIGNED_INT,
			(void*)0
		);
		//glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_data.size() / 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);*/

		if (deltaTime * 1000 >= 100) {
			//std::cout << "Delta time: " << deltaTime * 1000 <<  "ms" << std::endl;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwWindowShouldClose(window) == 0);
}