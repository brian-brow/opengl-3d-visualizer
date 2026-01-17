#include "Shader.h"
#include <iostream>

GLuint Shader::compileShader(GLenum type, const char* source) {
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
				char infoLog[512];
				glGetShaderInfoLog(shader, 512, nullptr, infoLog);
				std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
		}
		return shader;
}

Shader::Shader(const char* vertexSource, const char* fragmentSource) {
    // Compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    // Create shader program
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
		glUseProgram(programID);
}

void Shader::setMat4(const char* name, const glm::mat4& matrix) {
		GLint location = glGetUniformLocation(programID, name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const char* name, const glm::vec3& value) {
		GLint location = glGetUniformLocation(programID, name);
		glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setInt(const char* name, int value) {
    GLint location = glGetUniformLocation(programID, name);
    glUniform1i(location, value);
}

void Shader::setFloat(const char* name, float value) {
    GLint location = glGetUniformLocation(programID, name);
    glUniform1f(location, value);
}

Shader::~Shader() {
    glDeleteProgram(programID);
}
