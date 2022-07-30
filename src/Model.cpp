#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <streambuf>
#include "Model.hpp"
#include "objloader.hpp"

static float parse_float(std::ifstream& s) {
    char f_buf[sizeof(float)];
    s.read(f_buf, sizeof(float));
    float* fptr = reinterpret_cast<float*>(f_buf);
    return *fptr;
}

static glm::vec3 parse_vertex(std::ifstream& s)  {
    float x = parse_float(s);
    float y = parse_float(s);
    float z = parse_float(s);
    return glm::vec3(x,y,z);
}

void Model::reset() {
    vertices.clear();
    normals.clear();
    UVs.clear();
}

void Model::loadOBJ(const std::string& fileName) {
    reset();
    addOBJ(fileName);
    std::cout << "LoadOBJ: " << vertices.size() << " vertices, " << normals.size() << " normals, " << UVs.size() << " UVs" << std::endl;
}

void Model::addOBJ(const std::string& fileName) {
    /* const std::string file = std::string(OBJ_DIR) + fileName; */
    ::loadOBJ(fileName.c_str(), vertices, UVs, normals);
}

void Model::loadSTL(const std::string fileName) {
    reset();
    addSTL(fileName);
}

void Model::addSTL(const std::string fileName) {

    std::ifstream stl_file(fileName.c_str(), std::ios::in | std::ios::binary);
    if (!stl_file) {
        std::cerr << "ERROR: COULD NOT READ FILE " << fileName << std::endl;
        assert(false);
    }
    
    char header_info[80] = "";
    char n_triangles[4];
    stl_file.read(header_info, 80);
    stl_file.read(n_triangles, 4);
    std::string h(header_info);

    unsigned int* r = reinterpret_cast<unsigned int*>(n_triangles);
    for (unsigned int i = 0; i < *r; i++) {
        auto normal = parse_vertex(stl_file);
        auto v1 = parse_vertex(stl_file);
        auto v2 = parse_vertex(stl_file);
        auto v3 = parse_vertex(stl_file);
        char dummy[2];
        stl_file.read(dummy, 2);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
    }
}

void Model::loadTexture(const std::string& fileName) {
    texture = png(fileName);
}

const std::vector<glm::vec3>& Model::getVertices() {
    return vertices;
}

const std::vector<glm::vec3>& Model::getNormals() {
    return normals;
}

const std::vector<glm::vec2>& Model::getUVs() {
    return UVs;
}

const struct png& Model::getTexture() {
    return texture;
}

const std::string& Model::getName() const {
    return name;
}

std::ostream& operator<< (std::ostream& out, glm::vec3& v) {
    out << "[" << v.x << " " << v.y << " " << v.z << "]";
    return out;
}
