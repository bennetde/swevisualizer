#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

glm::mat4x4 Camera::getViewMatrix()
{
	return glm::translate(glm::lookAt(position,
									  position + front,
									  up),
						  glm::vec3(0.0f, 0.0f, -3.0f));
}

// void Camera::updateCamera(GLFWwindow *window, double lastX, double lastY, double xpos, double ypos, bool firstMouse, double sensitivity) {
// 	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
// 	{
// 		if (firstMouse)
// 		{
// 			lastX = xpos;
// 			lastY = ypos;
// 			firstMouse = false;
// 		}

// 		float xoffset = xpos - lastX;
// 		float yoffset = lastY - ypos;
// 		lastX = xpos;
// 		lastY = ypos;

// 		xoffset *= sensitivity;
// 		yoffset *= sensitivity;

// 		yaw += xoffset;
// 		pitch += yoffset;

// 		if (pitch > 89.0f)
// 			pitch = 89.0f;
// 		if (pitch < -89.0f)
// 			pitch = -89.0f;

// 		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
// 		direction.y = sin(glm::radians(pitch));
// 		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
// 		front = glm::normalize(direction);
// 	}

// 	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
// 		glfwSetWindowShouldClose(window, true);

// 	// todo: eventuell cameraspeed noch mit deltaTime anpassen
// 	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
// 		position += up * cameraSpeed;
// 	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
// 		position -= up * cameraSpeed;
// 	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
// 		position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
// 	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
// 		position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
// 	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
// 		position += cameraSpeed * front;
// 	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
// 		position -= cameraSpeed * front;
// }
