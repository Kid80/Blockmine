#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	glm::vec3 position = glm::vec3(0, 0, 0);
	float horizontalAngle = 0.0f;
	float verticalAngle = 0.0f;
	float FoV = 45.0f;
	float speed = 3.0f;
	float mouseSpeed = 0.005f;

	int width;
	int height;

	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	void getDirections();
	void move(float x, float y, float z);

	glm::mat4 getProjMat();
	glm::mat4 getViewMat();
};