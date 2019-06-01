#pragma once

#include "libs.h"

enum shader_enum { SHADER_CORE_PROGRAM = 0 };
enum texture_enum { TEX_GERALT0 = 0, TEX_GERALT0_SPECULAR, TEX_CONTAINER, TEX_CONTAINER_SPECULAR };
enum material_enum { MAT_0 = 0 };
enum mesh_enum { MESH_QUAD = 0 };

class Game {
private:

	GLFWwindow* window;
	const unsigned WINDOW_WIDTH;
	const unsigned WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	const unsigned GL_VERSION_MAJOR;
	const unsigned GL_VERSION_MINOR;

	float dt;
	float curTime;
	float lastTime;

	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;

	Camera camera;

	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;
	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	std::vector<Shader*> shaders;
	std::vector<Texture*> textures;
	std::vector<Material*> materials;
	std::vector<Mesh*> meshes;
	std::vector<Model*> models;
	std::vector<glm::vec3*> lights;

	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initMeshes();
	void initModels();
	void initLights();
	void initUniforms();

	void updateUniforms();

public:
	Game(const char* title, const unsigned WINDOW_WIDTH, const unsigned WINDOW_HEIGHT, const unsigned GL_VERSION_MAJOR, const unsigned GL_VERSION_MINOR, bool resizable);
	virtual ~Game();

	int getWindowShouldClose();

	void update();
	void render();
	void updateDt();
	void updateMouseInput();
	void updateKeyboardInput();
	void updateInput();

	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
};
