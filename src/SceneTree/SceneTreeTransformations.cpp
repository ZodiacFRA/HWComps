#include "SceneTree.hpp"


int SceneTree::translateNode(std::string nodeName, glm::vec3 tM)
{
	Node *node = getNode(nodeName);
	if (!node)
		return FAILURE;
	return translateNode(node, tM);
}

int SceneTree::translateNode(Node *node, glm::vec3 tM, bool onlyCheckCollision)
{
	if (node->checkCollisions) {
		glm::vec3 tmpPos = glm::translate(node->modelMatrix, tM)[3];
		for (auto tmpNode : _nodes) {
			if (tmpNode.second->name != node->name && tmpNode.second->checkCollisions) {
				int flag = isColliding(node, tmpPos, tmpNode.second);
				// printf("%s - %s\n", node->name.c_str(), tmpNode.first.c_str());
				if (flag) {
					if (flag == FLOOR_COLLIDING) {
						float minY1 = node->modelMatrix[3].y + node->obj->_mins.y;
						float maxY2 = tmpNode.second->modelMatrix[3].y + tmpNode.second->obj->_maxs.y;
						float deltaD = minY1 - maxY2 - 0.1;
						if (deltaD > 0 && deltaD > 0.1 && !onlyCheckCollision)
							node->modelMatrix = glm::translate(node->modelMatrix, glm::vec3(0, -deltaD, 0));
					}
					return flag;
				}
			}
		}
	}
	if (!onlyCheckCollision)
		node->modelMatrix = glm::translate(node->modelMatrix, tM);
	return NOT_COLLIDING;
}

int SceneTree::isColliding(Node *n1, glm::vec3 nextPos, Node *n2)
{
	// X
	float minX1 = nextPos.x + n1->obj->_mins.x;
	float maxX1 = nextPos.x + n1->obj->_maxs.x;
	float minX2 = n2->modelMatrix[3].x + n2->obj->_mins.x;
	float maxX2 = n2->modelMatrix[3].x + n2->obj->_maxs.x;
	// Y
	float minY1 = nextPos.y + n1->obj->_mins.y;
	float maxY1 = nextPos.y + n1->obj->_maxs.y;
	float minY2 = n2->modelMatrix[3].y + n2->obj->_mins.y;
	float maxY2 = n2->modelMatrix[3].y + n2->obj->_maxs.y;
	// Z
	float minZ1 = nextPos.z + n1->obj->_mins.z;
	float maxZ1 = nextPos.z + n1->obj->_maxs.z;
	float minZ2 = n2->modelMatrix[3].z + n2->obj->_mins.z;
	float maxZ2 = n2->modelMatrix[3].z + n2->obj->_maxs.z;
	if ((minX1 > maxX2 || maxX1 < minX2) ||
		(minZ1 > maxZ2 || maxZ1 < minZ2) ||
		(minY1 > maxY2 || maxY1 < minY2)) {
		return NOT_COLLIDING;
	}
	if (minY1 < maxY2) {
		return FLOOR_COLLIDING;
	}
	return COLLIDING;
}

int SceneTree::setNodePosition(std::string nodeName, glm::vec3 pM)
{
	std::cout << nodeName << '\n';
	printWarning("Warning! setNodePosition() used! No collisions check!");
	Node *node = NULL;
	auto it = _nodes.find(nodeName);
	if (it != _nodes.end())
		node = it->second;
	else
		return printError("Can't set Node position (Node not found)");
	glm::vec3 mM = node->modelMatrix[3];
	glm::vec3 tM(pM[0] - mM[0], pM[1] - mM[1], pM[2] - mM[2]);
	node->modelMatrix = glm::translate(node->modelMatrix, tM);
	return SUCCESS;
}

int SceneTree::rotateNode(std::string nodeName, float degrees, glm::vec3 rM)
{
	Node *node = NULL;
	auto it = _nodes.find(nodeName);
	if (it != _nodes.end())
		node = it->second;
	else
		return printError("Can't rotate node (Node not found)");
	// x, y, z are the axis of rotation
	node->modelMatrix = glm::rotate(node->modelMatrix,
					glm::radians(degrees), rM);
	return SUCCESS;
}

int SceneTree::rotateNodeRad(std::string nodeName, float rads, glm::vec3 rM)
{
	Node *node = NULL;
	auto it = _nodes.find(nodeName);
	if (it != _nodes.end())
		node = it->second;
	else
		return printError("Can't rotate node (Node not found)");
	// x, y, z are the axis of rotation
	node->modelMatrix = glm::rotate(node->modelMatrix,
					rads, rM);
	return SUCCESS;
}

int SceneTree::setNodeRotation(std::string nodeName, float degrees, glm::vec3 rM)
{
	// DOES NOT WORK
	// I think 3 glm::vec3 are needed to set the modelMatrix[1] and [2]
	Node *node = NULL;
	auto it = _nodes.find(nodeName);
	if (it != _nodes.end())
		node = it->second;
	else
		return printError("Can't set node rotation (Node not found)");
	glm::vec3 mM = node->modelMatrix[1];
	// glm::vec3 resM(rM[0] - mM[0], rM[1] - mM[1], rM[2] - mM[2]);
	node->modelMatrix = glm::rotate(node->modelMatrix,
					glm::radians(degrees), rM);
	return SUCCESS;
}


int SceneTree::scaleNode(std::string nodeName, glm::vec3 sM)
{
	Node *node = NULL;
	auto it = _nodes.find(nodeName);
	if (it != _nodes.end())
		node = it->second;
	else
		return printError("Can't rotate node (Node not found)");
	node->modelMatrix = glm::scale(node->modelMatrix, sM);
	return SUCCESS;
}


int SceneTree::setNodeScale(std::string nodeName, glm::vec3 sM)
{
	Node *node = NULL;
	auto it = _nodes.find(nodeName);
	if (it != _nodes.end())
		node = it->second;
	else
		return printError("Can't set node rotation (Node not found)");
	glm::vec3 mM = node->modelMatrix[0];
	glm::vec3 resM(sM[0] - mM[0], sM[1] - mM[1], sM[2] - mM[2]);
	node->modelMatrix = glm::scale(node->modelMatrix, resM);
	return SUCCESS;
}
