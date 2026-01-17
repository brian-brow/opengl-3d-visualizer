#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

class Mesh {
private:
    GLuint VAO, VBO, EBO;
    unsigned int indexCount;

public:
    Mesh(float* vertices, int vertSize, unsigned int* indices, int indSize);
    void draw();
    ~Mesh();
};

#endif
