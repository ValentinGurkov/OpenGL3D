#pragma once

#include<iostream>

#include"Shader.h"
#include"Texture.h"
#include"Material.h"
#include"Primitives.h"
#include"Vertex.h"

class Mesh {
private:
	unsigned numOfVertices;
	unsigned numOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 ModelMatrix;

	void initVAO(Vertex* vertexArray, const unsigned& numOfVertices, GLuint* indexArray, const unsigned& numOfIndices) {
		//SET VARIABLES
		this->numOfVertices = numOfVertices;
		this->numOfIndices = numOfIndices;

		//CREATE VAO
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GEN VBO AND SEND DATA

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->numOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

		//GEN EBO AND SEND DATA'
		if (this->numOfIndices > 0) {
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);
		}

		//SET VERTEX ATTRIBUTES AND ENABLE  - input assembly
		//Position
		//GLuint attribLocation = glGetAttribLocation(core_program, "vertex_position");
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		//GLuint attribLocation = glGetAttribLocation(core_program, "vertex_color");
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texcoord
		//GLuint attribLocation = glGetAttribLocation(core_program, "vertex_texcoord");
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		//GLuint attribLocation = glGetAttribLocation(core_program, "vertex_normal");
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//BIND VAO 0
		glBindVertexArray(0);
	}

	void initVAO(Primitive* primitive) {
		//SET VARIABLES
		this->numOfVertices = primitive->getNumOfVertices();
		this->numOfIndices = primitive->getNumOfIndices();

		//CREATE VAO
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GEN VBO AND SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->numOfVertices * sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW);

		//GEN EBO AND SEND DATA'
		if (this->numOfIndices > 0) {
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numOfIndices * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);
		}

		//SET VERTEX ATTRIBUTES AND ENABLE  - input assembly
		//Position
		//GLuint attribLocation = glGetAttribLocation(core_program, "vertex_position");
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		//GLuint attribLocation = glGetAttribLocation(core_program, "vertex_color");
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texcoord
		//GLuint attribLocation = glGetAttribLocation(core_program, "vertex_texcoord");
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		//GLuint attribLocation = glGetAttribLocation(core_program, "vertex_normal");
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		//BIND VAO 0
		glBindVertexArray(0);
	}

	void updateModelMatrix() {
		this->ModelMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	}

	void updateUniforms(Shader* shader) {
		shader->setMat4fv("ModelMatrix", this->ModelMatrix);
	}
public:

	Mesh(Primitive* primitive,
		glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)) {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		this->initVAO(primitive);
		this->updateModelMatrix();
	}

	Mesh(Vertex* vertexArray, const unsigned& numOfVertices, GLuint* indexArray, const unsigned& numOfIndices,
		glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)) {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		this->initVAO(vertexArray, numOfVertices, indexArray, numOfIndices);
		this->updateModelMatrix();
	}

	~Mesh() {
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		if (this->numOfIndices > 0) {
			glDeleteBuffers(1, &this->EBO);
		}
	}

	void setPosition(const glm::vec3 position) {
		this->position = position;
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

	void render(Shader* shader) {
		this->updateModelMatrix();
		this->updateUniforms(shader);

		shader->use();

		//BIND VERTEX ARRAY OBJECT
		glBindVertexArray(this->VAO);

		//DRAW
		if (this->numOfIndices == 0) {
			glDrawArrays(GL_TRIANGLES, 0, this->numOfVertices);
		}
		else {
			glDrawElements(GL_TRIANGLES, this->numOfIndices, GL_UNSIGNED_INT, 0);
		}
	}
};