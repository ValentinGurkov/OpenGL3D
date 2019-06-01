#pragma once
#include<iostream>
#include<string>
#include<fstream>

#include<glew.h>
#include<glfw3.h>

//GLM
#include<glm.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<mat4x4.hpp>
#include<gtc/type_ptr.hpp>

class Shader {
private:
	GLuint id = 0;
	const unsigned GLMajorV;
	const unsigned GLMinorV;

	std::string loadShaderSource(char* fileName) {
		char infoLog[512];
		GLint success = true;

		std::string temp = "";
		std::string src = "";
		std::fstream in_file;

		//Vertex
		in_file.open(fileName);

		if (in_file.is_open()) {
			while (std::getline(in_file, temp)) {
				src += temp + "\n";
			}
		} else {
			std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << fileName << "\n";
			success = false;
		}

		std::string version = std::to_string(this->GLMajorV) + std::to_string(this->GLMinorV) + "0";

		src.replace(src.find("#version"), 12, "#version " + version);

		in_file.close();

		return src;
	}

	GLuint loadShader(GLenum type, char* fileName) {
		char infoLog[512];
		GLint success = true;
		GLuint shader = glCreateShader(type);
		std::string shaderSource = this->loadShaderSource(fileName);
		const GLchar* src = shaderSource.c_str();
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
			std::cout << infoLog << "\n";
		}

		return shader;
	}

	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {
		char infoLog[512];
		GLint success = true;

		this->id = glCreateProgram();

		glAttachShader(this->id, vertexShader);

		if (geometryShader) {
			glAttachShader(this->id, geometryShader);
		}

		glAttachShader(this->id, fragmentShader);

		glLinkProgram(this->id);

		glGetProgramiv(this->id, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
			std::cout << infoLog << "\n";
		}

		glUseProgram(0);
	}
public:
	Shader(const unsigned GLMajorV, const unsigned GLMinorV, const char* vertexFile, const char* fragmentFile, const char* geometryFile = "") : GLMajorV(GLMajorV), GLMinorV(GLMinorV) {
		GLuint vertexShader = 0;
		GLuint geometryShader = 0;
		GLuint fragmentShader = 0;

		vertexShader = loadShader(GL_VERTEX_SHADER, (char*)vertexFile);
		if (geometryFile != "") {
			geometryShader = loadShader(GL_GEOMETRY_SHADER, (char*)geometryFile);
		}
		fragmentShader = loadShader(GL_FRAGMENT_SHADER, (char*)fragmentFile);

		this->linkProgram(vertexShader, geometryShader, fragmentShader);

		//End
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
	}

	~Shader() {
		glDeleteProgram(this->id);
	}

	void use() {
		glUseProgram(this->id);
	}

	void unuse() {
		glUseProgram(0);
	}

	void set1i(const GLchar * name, GLint value) {
		//maybe use/unuse
		glUniform1i(glGetUniformLocation(this->id, name), value);
	}

	void set1f(const GLchar * name, GLfloat value) {
		glUniform1f(glGetUniformLocation(this->id, name), value);
	}

	void setVec2f(const GLchar * name, glm::fvec2 value) {
		glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
	}

	void setVec3f(const GLchar * name, glm::fvec3 value) {
		glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
	}

	void setVec4f(const GLchar * name, glm::fvec4 value) {
		glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
	}

	void setMat3fv(const GLchar * name, glm::mat3 value, GLboolean transpose = GL_FALSE) {
		glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
	}

	void setMat4fv(const GLchar * name, glm::mat4 value, GLboolean transpose = GL_FALSE) {
		glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
	}
};