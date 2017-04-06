#include "glm.hpp"

#pragma once
class Camera
{
public:
	Camera() : theta(0), phi(-20), position(-10, 4, 0) {}
	~Camera();

	glm::mat4 getProjectionMatrix(float w, float h);

	glm::mat4 getViewMatrix();

	glm::vec3 position;

	void update();

private:
	float theta;
	float phi;
	

	int lastMouseX;
	int lastMouseY;
};

