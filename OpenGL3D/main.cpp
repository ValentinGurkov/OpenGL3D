#include "libs.h"

int main() {

	glfwInit();


	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw "Error in GLEW init!";
	}

	system("PAUSE");

	return 0;
}