#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	float cameraSpeed = 0.05f;
	float yaw = 90.0f;
	float pitch = 0.0f;

	glm::mat4x4 getViewMatrix();
	// void updateCamera(GLFWwindow *window, double lastX, double lastY, double xpos, double ypos, bool firstMouse, double sensitivity);
	// for resetting
	const glm::vec3 originalPosition = glm::vec3(0.0f, 0.0f, -10.0f);
	const glm::vec3 originalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 originalFront = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 direction = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
	glm::vec3 position = originalPosition;
	glm::vec3 up = originalUp;
	glm::vec3 front = originalFront;
};