#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "png_loader.hpp"

class Model {
protected:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;
    struct png texture;
    const std::string name;
public:
    // Assume that the .obj and the .png has the same name
    Model(const std::string& name): name(name) {}
    Model(): Model("") {}
    void loadSTL(const std::string fileName);
    void addSTL(const std::string fileName);
    void loadOBJ(const std::string& fileName);
    void addOBJ(const std::string& fileName);
    void loadTexture(const std::string& fileName);
    const std::vector<glm::vec3>& getVertices();
    const std::vector<glm::vec3>& getNormals();
    const std::vector<glm::vec2>& getUVs();
    const struct png& getTexture();
    const std::string& getName() const;
private:
    void reset();
};

std::ostream& operator<< (std::ostream& out, glm::vec3& v);
#endif
