#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

/**
 * Give possibility to move around to view plane
 */
class Camera
{
public:
	/**
	 * Variables cameraspeed, yaw (horizontal rotation) and pitch (vertical rotation) of camera position
	 */
	float cameraSpeed = 0.05f;
	float yaw = 0.0f;
	float pitch = 0.0f;

	/**
	 * Return view matrix to allow correct positioning and rendering ofthe image from the camera perspective
	 */
	glm::mat4x4 getViewMatrix();

	/**
	 * Constants for resetting camera position and orientation to origin
	 */
	const glm::vec3 originalPosition = glm::vec3(-30.0f, 30.0f, -100.0f);
	const glm::vec3 originalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 originalFront = glm::vec3(0.0f, 0.0f, 1.0f);

	/**
	 * Camera orientation (direction, position, up, front)
	 */
	glm::vec3 direction = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
	glm::vec3 position = originalPosition;
	glm::vec3 up = originalUp;
	glm::vec3 front = originalFront;
};