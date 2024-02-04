#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

/**
 * Returns the view matrix for the camera class using lookAt to create a first view matrix
 * and translate to create the finale view matrix that can be moved backwards along the negative z-axis
 */
glm::mat4x4 Camera::getViewMatrix()
{
	return glm::translate(glm::lookAt(position,
									  position + front,
									  up),
						  glm::vec3(0.0f, 0.0f, -3.0f));
}