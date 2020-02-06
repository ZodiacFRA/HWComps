#include "SceneTree.hpp"


int SceneTree::translateNode(std::string nodeName, glm::vec3 tM)
{
	Node *node = NULL;
	auto it = _nodes.find(nodeName);
	if (it != _nodes.end())
		node = it->second;
	else
		return printError("Can't translate node (Node not found)");
	if (node->checkCollisions) {
		glm::mat4 tmpM = glm::translate(node->modelMatrix, tM);
		for (auto tmpNode : _nodes) {
			if (tmpNode.second->name != node->name && tmpNode.second->checkCollisions &&
				isColliding(node, tmpM[3], tmpNode.second)) {
				glm::vec3 n1Pos = node->modelMatrix[3];
				glm::vec3 n2Pos = node->modelMatrix[3];
				glm::vec3 repulsionM = glm::vec3();
				node->modelMatrix = glm::translate(node->modelMatrix, repulsionM);
				return FAILURE;
			}
		}

	}
	node->modelMatrix = glm::translate(node->modelMatrix, tM);
	return SUCCESS;
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

	// printf("%f > %f || %f < %f\n", minX1, maxX2, maxX1, minX2);
	// printf("%f > %f || %f < %f\n", minY1, maxY2, maxY1, minY2);
	// printf("%f > %f || %f < %f\n", minZ1, maxZ2, maxZ1, minZ2);
	if (
		(minX1 > maxX2 || maxX1 < minX2) ||
		(minY1 > maxY2 || maxY1 < minY2) ||
		(minZ1 > maxZ2 || maxZ1 < minZ2)
	)
		return 0;
	return 1;
}

int SceneTree::setNodePosition(std::string nodeName, glm::vec3 pM)
{
	// No collisions check !
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
