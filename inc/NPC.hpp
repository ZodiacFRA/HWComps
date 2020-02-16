#include "common_HWComps.hpp"

struct NPC {
    Node *_node;
    std::vector<glm::vec3> _path;
    void updatePath(std::vector<glm::vec3> newPath)
    {
        _path = newPath;
    };
    glm::vec3 getNextMove()
    {
        if (!_path.size()) {
            return glm::vec3(0, 0, 0);
        }
        std::cout << "moving!" << '\n';
        return glm::vec3(
                        _path[1].x - _node->modelMatrix[3].x,
                        0,
                        _path[1].z - _node->modelMatrix[3].z);
    }
};
