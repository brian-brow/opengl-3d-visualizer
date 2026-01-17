#include "Texture.h"
#include <SDL2/SDL_image.h>
#include <iostream>

Texture::Texture(const char* filepath) : textureID(0), width(0), height(0) {
    // Load image with SDL_image
    SDL_Surface* surface = IMG_Load(filepath);
    if (!surface) {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        std::cerr << "SDL_image Error: " << IMG_GetError() << std::endl;
        return;
    }
    
    width = surface->w;
    height = surface->h;
    
    // Determine format based on bytes per pixel
    GLenum format;
    if (surface->format->BytesPerPixel == 4) {
        format = GL_RGBA;
    } else if (surface->format->BytesPerPixel == 3) {
        format = GL_RGB;
    } else {
        std::cerr << "Unsupported image format" << std::endl;
        SDL_FreeSurface(surface);
        return;
    }
    
    // Generate OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Cleanup
    SDL_FreeSurface(surface);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    std::cout << "Texture loaded: " << filepath << " (" << width << "x" << height << ")" << std::endl;
}

void Texture::bind(unsigned int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}
