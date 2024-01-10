#include "camera.h"
#include <glm/glm.hpp>

glm::vec3 Camera::getDirection() {
	return glm::normalize(this->position - this->target);
}

glm::mat4x4 Camera::getViewMatrix() {
	return glm::mat4x4(1.0f);
}