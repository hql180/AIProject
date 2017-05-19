#include "glm.hpp"

#pragma once
class Camera
{
public:
	Camera(glm::vec3 pos = glm::vec3(0), float t = 0, float p = -20);

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

