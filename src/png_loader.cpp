#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "asset.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "png_loader.hpp"
    
png::png(const std::string& fileName) {
    data = stbi_load(fileName.c_str(), &x, &y, &n, 0);

    std::cout << "load_texture_from_png: Width: " << x << " Height: " << y << " Channels: " << n << std::endl;
}

png::~png() {
    /* stbi_image_free(data); */
}

GLuint load_texture_from_png(const std::string& file_name) {
    int x,y,n;
    unsigned char *data = stbi_load(file_name.c_str(), &x, &y, &n, 0);
    std::cout << "load_texture_from_png: Width: " << x << " Height: " << y << " Channels: " << n << std::endl;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D); // Unavailable in OpenGL 2.1, use gluBuild2DMipmaps() instead

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return textureID;
}

int test() {
    int x,y,n;
    unsigned char *data = stbi_load(TEXTURE_DIR "/Material.001 Base Color.png", &x, &y, &n, 0);

    std::cout << "Width: " << x << " Height: " << y << " Channels: " << n << std::endl;
    std::cout << "Read first 10 pixels:" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << "[ ";
        for (int j = 0; j < n; j++) {
            unsigned char value = data[i*n+j];
            std::cout << static_cast<int>(value) << " ";
        }
        std::cout << "]" << std::endl;
    }
    stbi_image_free(data);
}
