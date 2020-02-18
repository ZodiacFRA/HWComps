#include "NPC.hpp"


void NPC::updatePath(std::list<glm::vec3> newPath)
{
    _path = newPath;
	if (_path.size())
    	_path.pop_front();
}

glm::vec3 NPC::getNextMove(float &totalTurnDistance)
{
    if (!_path.size()) {
		// std::cout << "no path" << '\n';
		totalTurnDistance = -1.0;
		return glm::vec3(0, 0, 0);
	}
    glm::vec3 target = glm::vec3(
                    _path.front().x - _node->modelMatrix[3].x,
                    0,
                    _path.front().z - _node->modelMatrix[3].z);

	// totalTurnDistance = -1.0;

    float targetDistance = (float)sqrt(pow(target.x, 2) + pow(target.z, 2));
	// Target is too far away, cap movement
	if (targetDistance > _speed) {
		target /= targetDistance;
		target *= _speed;
		totalTurnDistance += _speed;
		return target;
	}
	// We need to move again to the next target, or the targetDistance = _speed
	if (targetDistance <= _speed) {
		totalTurnDistance += targetDistance;
		_path.pop_front();
		return target;
	}
	std::cout << "NPC getNextMove error" << '\n';
	totalTurnDistance = -1.0;
	return target;
}
