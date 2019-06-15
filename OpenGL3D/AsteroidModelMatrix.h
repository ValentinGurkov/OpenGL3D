#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class AsteroidModelMatrix {
private:
	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 matrix;
	float rotAngle;
	glm::vec3 translate;
	glm::vec3 oldAxis;
public:
	AsteroidModelMatrix(glm::vec3 position = glm::vec3(0.f), glm::vec3 origin = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)) {
		this->position = position;
		this->origin = origin;
		this->rotation = rotation;
		this->scale = scale;
	}

	~AsteroidModelMatrix() {}

	void initModelMatrix(glm::vec3 position, glm::vec3 scale, float rotAngle, glm::vec3 rotation) {
		this->position = position;
		this->rotAngle = rotAngle;
		this->rotation = rotation;
		this->oldAxis = rotation;
		this->scale = scale;
		this->matrix = glm::mat4(1.f);
		this->matrix = glm::translate(this->matrix, this->position);
		this->matrix = glm::scale(this->matrix, this->scale);
		this->matrix = glm::rotate(this->matrix, this->rotAngle, this->rotation);
	}

	void updateModelMatrix() {
		this->matrix = glm::mat4(1.f);
		this->matrix = glm::translate(this->matrix, this->origin);
		this->matrix = glm::rotate(this->matrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->matrix = glm::rotate(this->matrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->matrix = glm::rotate(this->matrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->matrix = glm::translate(this->matrix, this->position - this->origin);
		this->matrix = glm::scale(this->matrix, this->scale);
		this->matrix = glm::rotate(this->matrix, this->rotAngle, this->oldAxis);
	}

	void setPosition(const glm::vec3 position) {
		this->position = position;
	}

	void setOrigin(const glm::vec3 origin) {
		this->origin = origin;
	}

	void setRotation(const glm::vec3 rotation) {
		this->rotation = rotation;
	}

	void setScale(const glm::vec3 scale) {
		this->scale = scale;
	}

	void move(const glm::vec3 position) {
		this->position += position;
	}

	void rotate(const glm::vec3 rotation) {
		this->rotation += rotation;
	}

	void changeSacle(const glm::vec3 scale) {
		this->scale += scale;
	}

	glm::mat4 getModelMatrix() const {
		return this->matrix;
	}
};