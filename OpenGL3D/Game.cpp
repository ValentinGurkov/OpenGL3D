#include "Game.h"

void Game::initGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "ERORR:GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initWindow(const char* title, bool resizable) {
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr) {
		std::cout << "ERORR:GLFW_INIT_WINDOW_FAILED" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_resize_callback);

	glfwMakeContextCurrent(this->window);
}

void Game::initGLEW() {
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initMatrices() {
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);
	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);
}

void Game::initShaders() {
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR, "vertex_core.glsl", "fragment_core.glsl"));
}

void Game::initTextures() {
	this->textures.push_back(new Texture("Images/Geralt1.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/Geralt1_specular.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/container.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/container_specular.png", GL_TEXTURE_2D));
}

void Game::initMaterials() {
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));
}

void Game::initMeshes() {
	this->meshes.push_back(new Mesh(&Pyramid()));
	this->meshes.push_back(new Mesh(&Quad()));
}

void Game::initModels() {
	this->models.push_back(new Model(glm::vec3(0.f), this->materials[MAT_0], this->textures[TEX_CONTAINER], this->textures[TEX_CONTAINER_SPECULAR], this->meshes));
}

void Game::initLights() {
	this->lights.push_back(new glm::vec3(0.f, 0.f, 2.f));
}

void Game::initUniforms() {
	this->shaders[SHADER_CORE_PROGRAM]->use();
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv("ViewMatrix", this->ViewMatrix);
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv("ProjectionMatrix", this->ProjectionMatrix);

	this->shaders[SHADER_CORE_PROGRAM]->setVec3f("lightPos0", *this->lights[0]);

	glUseProgram(0);
}

void Game::updateUniforms() {
	//Adjust perspective on resisze
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	this->ViewMatrix = this->camera.getViewMatrix();
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv("ViewMatrix", ViewMatrix);
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f("camPosition", this->camera.getPosition());

	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv("ProjectionMatrix", this->ProjectionMatrix);
}

Game::Game(const char* title, const unsigned WINDOW_WIDTH, const unsigned WINDOW_HEIGHT, const unsigned GL_VERSION_MAJOR, const unsigned GL_VERSION_MINOR, bool resizable)
	: WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT), GL_VERSION_MAJOR(GL_VERSION_MAJOR), GL_VERSION_MINOR(GL_VERSION_MINOR),
	camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f)) {
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;

	this->dt = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;
	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->camPosition = glm::vec3(0.f, 0.f, 1.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);
	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initMeshes();
	this->initModels();
	this->initLights();
	this->initUniforms();
}

Game::~Game() {
	for (size_t i = 0; i < this->shaders.size(); i++) {
		delete this->shaders[i];
	}

	for (size_t i = 0; i < this->textures.size(); i++) {
		delete this->textures[i];
	}

	for (size_t i = 0; i < this->materials.size(); i++) {
		delete this->materials[i];
	}

	for (size_t i = 0; i < this->meshes.size(); i++) {
		delete this->meshes[i];
	}

	for (auto*& i : models) {
		delete i;
	}

	for (size_t i = 0; i < this->lights.size(); i++) {
		delete this->lights[i];
	}

	glfwDestroyWindow(this->window);
	glfwTerminate();
}

int Game::getWindowShouldClose() {
	return glfwWindowShouldClose(this->window);
}

void Game::render() {
	this->updateInput();

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	this->updateUniforms();

	/*this->materials[MAT_0]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

	this->shaders[SHADER_CORE_PROGRAM]->use();

	this->textures[TEX_CONTAINER]->bind(0);
	this->textures[TEX_CONTAINER_SPECULAR]->bind(1);
	this->meshes[MESH_QUAD]->render(this->shaders[SHADER_CORE_PROGRAM]);*/

	//this->models[0]->render(this->shaders[SHADER_CORE_PROGRAM]);

	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::updateDt() {
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

void Game::updateMouseInput() {
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);
	if (this->firstMouse) {
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}

void Game::updateKeyboardInput() {
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(this->window, GLFW_TRUE);
	}

	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
		this->camera.move(dt, FORWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		this->camera.move(dt, BACKWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		this->camera.move(dt, LEFT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		this->camera.move(dt, RIGHT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS) {
		this->camPosition.y += 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		this->camPosition.y -= 0.05f;
	}
}

void Game::updateInput() {
	glfwPollEvents();
	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::update() {
	this->updateDt();
	this->updateInput();
}
void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}