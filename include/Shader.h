#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
		GLuint programID;

		GLuint compileShader(GLenum type, const char* source);

public:
		Shader(const char* vertexSource, const char* fragmentSource);
		void use();
    void setMat4(const char* name, const glm::mat4& matrix);
    void setInt(const char* name, int value);
    void setFloat(const char* name, float value);
		void setVec3(const char* name, const glm::vec3& value);
    GLuint getID() const { return programID; }
    ~Shader();
};

#endif
