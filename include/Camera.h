#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    // Camera attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    // Euler angles
    float yaw;
    float pitch;
    
    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    
    // Update camera vectors based on euler angles
    void updateCameraVectors();
public:
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f);

		glm::mat4 getViewMatrix();

		void moveForward(float deltaTime);
		void moveBackward(float deltaTime);
		void moveLeft(float deltaTime);
		void moveRight(float deltaTime);
		void moveUp(float deltaTime);
		void moveDown(float deltaTime);

		void rotate(float xOffset, float yOffset, bool constrainPitch = true);

		glm::vec3 getPosition() const { return position; }
		glm::vec3 getFront() const { return front; }

		void setPosition(glm::vec3 pos) { position = pos; }
		void setYPosition(float yPos) { position.y = yPos; }
		void setSpeed(float speed) { movementSpeed = speed; }
		void setSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }
};

#endif
