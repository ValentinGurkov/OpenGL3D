#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	float yaw;
	float pitch;
	const float MovementSpeed;
	const float MouseSensitivity;
	float zoom;
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors() {
		// Calculate the new Front vector
		glm::vec3 frontVec;
		frontVec.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		frontVec.y = sin(glm::radians(this->pitch));
		frontVec.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(frontVec);
		// Also re-calculate the Right and Up vector
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
public:

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}

	~Camera() {}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD) {
			this->position += this->front * velocity;
		}
		else if (direction == BACKWARD) {
			this->position -= this->front * velocity;
		}
		else if (direction == LEFT) {
			this->position -= this->right * velocity;
		}
		else if (direction == RIGHT) {
			this->position += this->right * velocity;
		}
	}

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		this->yaw += xoffset;
		this->pitch += yoffset;

		if (constrainPitch) {
			if (this->pitch > 89.0f) {
				this->pitch = 89.0f;
			}
			else if (this->pitch < -89.0f) {
				this->pitch = -89.0f;
			}
		}

		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset) {
		if (this->zoom >= 1.0f && this->zoom <= 45.0f) {
			this->zoom -= yoffset;
		}
		else if (this->zoom <= 1.0f) {
			this->zoom = 1.0f;
		}
		else if (this->zoom >= 45.0f) {
			this->zoom = 45.0f;
		}
	}

	inline glm::mat4 GetViewMatrix() { return glm::lookAt(this->position, this->position + this->front, this->up); }
};