#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	glm::vec3 position;
	float horizontalAngle;
	float verticalAngle;
	float FoV;
	float speed;
	float mouseSpeed;

	int width;
	int height;

	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	void getDirections();
	void move(float x, float y, float z);

	glm::mat4 getProjMat();
	glm::mat4 getViewMat();

	Camera();
	~Camera();
};