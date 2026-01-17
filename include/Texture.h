#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

class Texture {
private:
    GLuint textureID;
    int width, height;

public:
    Texture(const char* filepath);
    void bind(unsigned int slot = 0);
    void unbind();
    GLuint getID() const { return textureID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    ~Texture();
};

#endif
