#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player {
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

	double cursorXpos;
	double cursorYpos;
	double cursorXposOld;
	double cursorYposOld;
	double cursorXdelta;
	double cursorYdelta;

	void* window;

	void getDirections();
	void move(float x, float y, float z);
	void update(float deltaTime);

	glm::mat4 getProjMat();
	glm::mat4 getViewMat();

	
	void init(void* window, glm::vec3 position = glm::vec3(0, 2, 0));
	void onRightClick();
	~Player();
};