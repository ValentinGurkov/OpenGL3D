#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class ModelMatrix {
private:
	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 matrix = glm::mat4(1.f);
public:
	ModelMatrix(glm::vec3 position = glm::vec3(0.f), glm::vec3 origin = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)) {
		//this->position = position;
		this->origin = origin;
		//this->rotation = rotation;
		//this->scale = scale;
	}

	~ModelMatrix() {}

	void updateModelMatrix() {
		this->matrix = glm::mat4(1.f);
		this->matrix = glm::translate(this->matrix, this->origin);
		this->matrix = glm::rotate(this->matrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->matrix = glm::rotate(this->matrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->matrix = glm::rotate(this->matrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->matrix = glm::translate(this->matrix, this->position - this->origin);
		this->matrix = glm::scale(this->matrix, this->scale);
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

	void setModelMatrix(const glm::mat4 modelMatrix) {
		this->matrix = modelMatrix;
	}

	glm::mat4 getModelMatrix() const {
		return this->matrix;
	}
};