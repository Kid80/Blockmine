#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "player.hpp"
#include <iostream>

void Player::getDirections() {
	//std::cout << verticalAngle << std::endl;
	if (verticalAngle >  1.0f) {
		verticalAngle = 1.0f;
	}
	if (verticalAngle < -1.0f) {
		verticalAngle = -1.0f;
	}

	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	up = glm::cross(right, direction);
}

void Player::move(float x, float y, float z) {
	position += direction * x;
	position += right * y;
	position += up * z;
}

glm::mat4 Player::getProjMat() {
	return glm::perspective(glm::radians(FoV), (float)width / (float)height, 0.1f, 100.0f);
}

glm::mat4 Player::getViewMat() {
	return glm::lookAt(
		position,
		position + direction,
		up
	);
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

void Player::update(float deltaTime) {
	glfwGetCursorPos((GLFWwindow*)window, &cursorXpos, &cursorYpos);
	glfwGetWindowSize((GLFWwindow*)window, &width, &height);

	if (glfwGetInputMode((GLFWwindow*)window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
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
	kbdX += glfwGetKey((GLFWwindow*)window, GLFW_KEY_W);
	kbdX -= glfwGetKey((GLFWwindow*)window, GLFW_KEY_S);
	float kbdY = 0.0f;
	kbdY += glfwGetKey((GLFWwindow*)window, GLFW_KEY_A);
	kbdY -= glfwGetKey((GLFWwindow*)window, GLFW_KEY_D);
	float kbdZ = 0.0f;
	kbdZ += glfwGetKey((GLFWwindow*)window, GLFW_KEY_Q);
	kbdZ -= glfwGetKey((GLFWwindow*)window, GLFW_KEY_E);
	horizontalAngle -= 0.5f * deltaTime * (float)cursorXdelta;
	verticalAngle -= 0.5f * deltaTime * (float)cursorYdelta;

	getDirections();

	float speed = 5.0f;
	move(
		kbdX * deltaTime * speed,
		kbdY * deltaTime * speed,
		kbdZ * deltaTime * speed
	);
}

Player::Player(void* window) {
	position = glm::vec3(0, 0, 0);
	horizontalAngle = 0.0f;
	verticalAngle = 0.0f;
	FoV = 45.0f;
	speed = 3.0f;
	mouseSpeed = 0.005f;

	width = 0;
	height = 0;

	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	up = glm::cross(right, direction);
	this->window = window;
}

Player::~Player() {

}