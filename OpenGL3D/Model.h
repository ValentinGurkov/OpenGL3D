#pragma once

#include"Mesh.h"
#include"Material.h"
#include"Shader.h"
#include"Texture.h"

class Model {
private:
	glm::vec3 position;
	Material* material;
	Texture* overriteTextureDiffuse;
	Texture* overrideTextureSpecular;
	std::vector<Mesh*> meshes;

public:
	Model(glm::vec3 position, Material* material, Texture* overrideTexDif, Texture* overrideTexSpec, std::vector<Mesh*> meshes) {
		this->position = position;
		this->material = material;
		this->overriteTextureDiffuse = overrideTexDif;
		this->overrideTextureSpecular = overrideTexSpec;

		for (auto* i : meshes) {
			this->meshes.push_back(new Mesh(*i));
		}

		for (auto* i : this->meshes) {
			i->move(this->position);
			i->setOrigin(this->position);
		}
	}

	~Model() {
		for (auto*& i : this->meshes) {
			delete i;
		}
	}

	void rotate(const glm::vec3 rotation) {
		for (auto& i : this->meshes) {
			i->rotate(rotation);
		}
	}

	void render(Shader* shader) {
		this->material->sendToShader(*shader);

		shader->use();

		this->overriteTextureDiffuse->bind(0);
		this->overrideTextureSpecular->bind(1);
		for (auto* i : this->meshes) {
			i->render(shader);
		}
	}
};