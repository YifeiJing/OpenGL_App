#ifndef PNG_LOADER_HPP
#define PNG_LOADER_HPP

#include <GLFW/glfw3.h>
GLuint load_texture_from_png(const std::string& file_name);

struct png {
    int x,y,n;
    unsigned char* data;
    png() = default;
    png(const std::string& fileName);
    ~png();
};

#endif
