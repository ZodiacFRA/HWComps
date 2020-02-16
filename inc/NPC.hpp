#include "common_HWComps.hpp"
#include "SceneTree.hpp"

class NPC {
public:
    Node *_node;
    std::vector<glm::vec3> _path;
    float speed = 0.1;

    void updatePath(std::vector<glm::vec3> newPath);
    glm::vec3 getNextMove();
};
