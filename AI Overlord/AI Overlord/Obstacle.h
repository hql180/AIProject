#pragma once

#include "glm.hpp"

enum Shape
{
	Box,
	Sphere
};

class Obstacle
{
public:
	glm::vec3 position;
	glm::vec3 extent;
	float radius;
	Shape shape;
};

