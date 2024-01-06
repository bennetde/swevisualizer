#include "camera.h"
#include <glm/glm.hpp>

glm::vec3 Camera::getDirection() {
	return glm::normalize(this->position - this->target);
}