#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
		: position(position), worldUp(up), yaw(yaw), pitch(pitch),
		  movementSpeed(2.5f), mouseSensitivity(0.1f) {
		updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
		return glm::lookAt(position, position + front, up);
}

void Camera::moveForward(float deltaTime) {
		glm::vec3 horizontalFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    position += horizontalFront * movementSpeed * deltaTime;
}

void Camera::moveBackward(float deltaTime) {
		glm::vec3 horizontalFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    position -= horizontalFront * movementSpeed * deltaTime;
}

void Camera::moveLeft(float deltaTime) {
		glm::vec3 horizontalFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
		glm::vec3 horizontalRight = glm::normalize(glm::cross(horizontalFront, worldUp));
    position -= horizontalRight * movementSpeed * deltaTime;
}

void Camera::moveRight(float deltaTime) {
		glm::vec3 horizontalFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
		glm::vec3 horizontalRight = glm::normalize(glm::cross(horizontalFront, worldUp));
    position += horizontalRight * movementSpeed * deltaTime;
}

void Camera::moveUp(float deltaTime) {
    position += up * movementSpeed * deltaTime;
}

void Camera::moveDown(float deltaTime) {
    position -= up * movementSpeed * deltaTime;
}

void Camera::rotate(float xOffset, float yOffset, bool constrainPitch) {
		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (constrainPitch) {
				if(pitch > 89.0f) { pitch = 89.0f; }
				if (pitch < -89.0f) { pitch = -89.0f; }
		}

		updateCameraVectors();
}

void Camera::updateCameraVectors() {
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
