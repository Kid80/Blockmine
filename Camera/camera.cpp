#include "camera.h"
#include <iostream>

void Camera::getDirections() {
	std::cout << verticalAngle << std::endl;
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

void Camera::move(float x, float y, float z) {
	position += direction * x;
	position += right * y;
	position += up * z;
}

glm::mat4 Camera::getProjMat() {
	return glm::perspective(glm::radians(FoV), (float)width / (float)height, 0.1f, 100.0f);
}

glm::mat4 Camera::getViewMat() {
	return glm::lookAt(
		position,
		position + direction,
		up
	);
}