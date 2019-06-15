#pragma once

#include <glew.h>
#include <string>

struct Texture {
	GLuint id;
	std::string type;
	std::string path;
};