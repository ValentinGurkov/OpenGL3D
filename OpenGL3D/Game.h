#pragma once

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>

enum shader_enum { SHADER_CORE_PROGRAM = 0 };
enum model_enum { MODEL_PLANET = 0, MODEL_ASTEROID };

class Game {
private:

	GLFWwindow* window;
	std::vector<Shader*> shaders;
	std::vector<Model*> models;
	const unsigned WINDOW_WIDTH;
	const unsigned WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	const unsigned GL_VERSION_MAJOR;
	const unsigned GL_VERSION_MINOR;

	float deltaTime;
	float lastFrame;
	float currentFrame;

	unsigned int amount;
	glm::mat4* modelMatrices;

	float radius;
	float offset;

	glm::mat4 projection;
	glm::mat4 view;

	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOptions();
	void initShaders();
	void initModels();
	void initAsteroidMMs();

public:
	Game(const char* title, const unsigned WINDOW_WIDTH, const unsigned WINDOW_HEIGHT, const unsigned GL_VERSION_MAJOR, const unsigned GL_VERSION_MINOR, bool resizable);
	virtual ~Game();

	int getWindowShouldClose();

	void update();
	void render();
	void updateInput();
	void updateDt();

	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};
