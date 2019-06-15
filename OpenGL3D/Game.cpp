#include "Game.h"
Camera camera(glm::vec3(0.0f, 0.0f, 55.0f));
float lastX;
float lastY;
bool firstMouse;

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
	glfwSetFramebufferSizeCallback(this->window, this->framebuffer_resize_callback);
	glfwSetCursorPosCallback(this->window, this->mouse_callback);
	glfwSetScrollCallback(this->window, this->scroll_callback);
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

void Game::initShaders() {
	this->shaders.push_back(new Shader("instancing.vs", "instancing.fs"));
}

void Game::initModels() {
	this->models.push_back(new Model("resources/objects/planet/planet.obj"));
	this->models.push_back(new Model("resources/objects/rock/rock.obj"));

	this->models[MODEL_PLANET]->setOrigin(glm::vec3(0.0f, -3.0f, 0.0f));
	this->models[MODEL_PLANET]->setScale(glm::vec3(4.0f, 4.0f, 4.0f));
}

void Game::initAsteroidMMs() {
	srand(glfwGetTime()); // initialize random seed

	for (unsigned int i = 0; i < this->amount; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)this->amount * 360.0f;
		float displacement = (rand() % (int)(2 * this->offset * 100)) / 100.0f - this->offset;
		float x = sin(angle) * this->radius + displacement;
		displacement = (rand() % (int)(2 * this->offset * 100)) / 100.0f - this->offset;
		float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * this->offset * 100)) / 100.0f - this->offset;
		float z = cos(angle) * this->radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		modelMatrices[i].setPosition(glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		modelMatrices[i].setScale(glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		modelMatrices[i].setRotation(glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		//modelMatrices[i].setModelMatrix(model);
	}
}

Game::Game(const char* title, const unsigned WINDOW_WIDTH, const unsigned WINDOW_HEIGHT, const unsigned GL_VERSION_MAJOR, const unsigned GL_VERSION_MINOR, bool resizable)
	:WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT), GL_VERSION_MAJOR(GL_VERSION_MAJOR), GL_VERSION_MINOR(GL_VERSION_MINOR)//, camera(glm::vec3(0.0f, 0.0f, 55.0f)) {
{
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;
	this->deltaTime = 0.f;
	this->lastFrame = 0.f;
	this->amount = 1000;
	this->modelMatrices = new ModelMatrix[amount];

	this->radius = 50.0;
	this->offset = 2.5f;

	lastX = (float)this->WINDOW_WIDTH / 2.0;
	lastY = (float)this->WINDOW_HEIGHT / 2.0;
	firstMouse = true;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();
	this->initShaders();
	this->initModels();
	this->initAsteroidMMs();
}

Game::~Game() {
	for (size_t i = 0; i < this->shaders.size(); i++) {
		delete this->shaders[i];
	}

	for (size_t i = 0; i < this->models.size(); i++) {
		delete this->models[i];
	}

	delete[] this->modelMatrices;
}

int Game::getWindowShouldClose() {
	return glfwWindowShouldClose(this->window);
}

void Game::render() {
	// render
	// ------
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// configure transformation matrices
	this->projection = glm::perspective(glm::radians(45.0f), (float)this->WINDOW_WIDTH / (float)this->WINDOW_HEIGHT, 0.1f, 1000.0f);
	this->view = camera.GetViewMatrix();;
	this->shaders[SHADER_CORE_PROGRAM]->use();
	this->shaders[SHADER_CORE_PROGRAM]->setMat4("projection", this->projection);
	this->shaders[SHADER_CORE_PROGRAM]->setMat4("view", this->view);

	// draw planet
	this->models[MODEL_PLANET]->updateModelMatrix();
	this->shaders[SHADER_CORE_PROGRAM]->setMat4("model", this->models[MODEL_PLANET]->getModelMatrix());
	this->models[MODEL_PLANET]->Draw(*this->shaders[SHADER_CORE_PROGRAM]);

	// draw meteorites
	for (unsigned int i = 0; i < amount; i++) {
		modelMatrices[i].updateModelMatrix();
		this->shaders[SHADER_CORE_PROGRAM]->setMat4("model", modelMatrices[i].getModelMatrix());
		this->models[MODEL_ASTEROID]->Draw(*this->shaders[SHADER_CORE_PROGRAM]);
	}

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(this->window);
}
void Game::update() {
	glfwPollEvents();
	this->updateDt();
	this->updateInput();
	this->models[MODEL_PLANET]->rotate(glm::vec3(0.f, 0.03f, 0.f));
	for (unsigned int i = 0; i < amount; i++) {
		modelMatrices[i].rotate(glm::vec3(0.f, 0.06f, 0.f));
	}
}

void Game::updateInput() {
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(this->window, true);
	}

	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, this->deltaTime);
	}

	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, this->deltaTime);
	}

	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, this->deltaTime);
	}

	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, this->deltaTime);
	}
}

void Game::updateDt() {
	this->currentFrame = glfwGetTime();
	this->deltaTime = this->currentFrame - this->lastFrame;
	this->lastFrame = this->currentFrame;
}

void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void Game::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}