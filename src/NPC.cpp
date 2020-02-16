#include "NPC.hpp"


void NPC::updatePath(std::vector<glm::vec3> newPath)
{
    std::cout << "path length = " << newPath.size() << '\n';
    _path = newPath;
    _path.erase(_path.begin());
};

glm::vec3 NPC::getNextMove()
{
    if (!_path.size()) {
        return glm::vec3(0, 0, 0);
    }
    glm::vec3 target = glm::vec3(
                    _path[0].x - _node->modelMatrix[3].x,
                    0,
                    _path[0].z - _node->modelMatrix[3].z);
    float length = (float)sqrt(pow(target.x, 2) + pow(target.z, 2));
    glm::vec3 finalTarget = target / length;
    finalTarget *= speed;
    float finalLength = (float)sqrt(pow(target.x, 2) + pow(target.z, 2));
    if (finalLength >= length) {
        _path.erase(_path.begin());
        return target;
    }
    return finalTarget;
}
