#pragma once

#include<glew.h>
#include<glfw3.h>

#include<glm.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<mat4x4.hpp>
#include<gtc/type_ptr.hpp>

#include"Shader.h"

class Material {
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLint diffuseTexture;
	GLint specularTexture;
public:
	Material(
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		GLint diffuseTexture,
		GLint specularTexture) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTexture = diffuseTexture;
		this->specularTexture = specularTexture;
	}

	~Material() {}

	void sendToShader(Shader& program) {
		program.setVec3f("material.ambient", this->ambient);
		program.setVec3f("material.diffuse", this->diffuse);
		program.setVec3f("material.specular", this->specular);
		program.set1i("material.diffuseTexture", this->diffuseTexture);
		program.set1i("material.specularTexture", this->specularTexture);
	}
};