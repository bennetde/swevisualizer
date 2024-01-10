#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
class Camera {
	glm::vec3 position;
	glm::vec3 target;

	glm::vec3 getDirection();
	glm::mat4x4 getViewMatrix();
};