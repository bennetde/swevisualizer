#pragma once
#include <glm/vec3.hpp>

class Camera {
	glm::vec3 position;
	glm::vec3 target;

	glm::vec3 getDirection();
};