#include "common_HWComps.hpp"
#include "SceneTree.hpp"

class NPC {
public:
    Node *_node;
    std::list<glm::vec3> _path;
    float _speed = 0.4;
	glm::vec3 _targetPosDelta;

    void updatePath(std::list<glm::vec3> newPath);
	glm::vec3 getNextMove(float &totalTurnDistance);
};
