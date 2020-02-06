#include "SceneTree.hpp"


SceneTree::SceneTree()
	: _lightPos(-10, 5, 0)
{
	_root.name = "";
}

SceneTree::~SceneTree()
{
	for (auto it : _nodes) {
		if (it.second)
			delete it.second;
	}
}


int SceneTree::applyGravity(float gravityValue)
{
	for (auto node : _nodes) {
		if (node.second->applyGravity) {
			float flag = translateNode(node.second, glm::vec3(0, gravityValue, 0));
			if (node.first == "PlayerNode") {
				if (flag == 2)
					return 1;
				else
					return 0;
			}
		}
	}
	return SUCCESS;
}


Node *SceneTree::insert(std::string parentName, std::string name, Obj *obj,
			Shader *shader, Texture *texture, glm::vec3 position,
			int checkCollisions, int applyGravity, int randomID)
{
	if (_nodes.find(name) != _nodes.end()) {
		printError("Node name already taken");
		return NULL;
	}
	Node *parentNode;
	if (parentName == "")  // TERNAIRE
		parentNode = &_root;
	else
		parentNode = _nodes[parentName];
	Node *newNode = new Node;
	newNode->name = name;
	newNode->checkCollisions = checkCollisions;
	newNode->applyGravity = applyGravity;
	newNode->modelMatrix = glm::translate(glm::mat4(1.0), position);
	newNode->randomID = randomID;
	newNode->parent = parentNode;
	newNode->obj = obj;
	newNode->shader = shader;
	newNode->texture = texture;
	parentNode->childs.push_back(newNode);
	_nodes.emplace(name, newNode);
	return newNode;
}


Node *SceneTree::insert(std::string parentName, std::string name, Obj *obj,
			Shader *shader, Texture *texture, glm::vec3 position,
			glm::vec3 rM, float rotationAngle, int checkCollisions,
			int applyGravity, int randomID)
{
	if (_nodes.find(name) != _nodes.end()) {
		printError("Node name already taken");
		return NULL;
	}
	Node *parentNode;
	if (parentName == "")  // TERNAIRE
		parentNode = &_root;
	else
		parentNode = _nodes[parentName];
	Node *newNode = new Node;
	newNode->name = name;
	newNode->checkCollisions = checkCollisions;
	newNode->applyGravity = applyGravity;
	newNode->modelMatrix = glm::translate(glm::mat4(1.0), position);
	newNode->modelMatrix = 	glm::rotate(
			newNode->modelMatrix, rotationAngle, rM
		),
	newNode->randomID = randomID;
	newNode->parent = parentNode;
	newNode->obj = obj;
	newNode->shader = shader;
	newNode->texture = texture;
	parentNode->childs.push_back(newNode);
	_nodes.emplace(name, newNode);
	return newNode;
}


int SceneTree::remove(std::string nodeName)
{
	Node *node = NULL;
	auto it = _nodes.find(nodeName);
	if (it != _nodes.end())
		node = it->second;
	else
		return printError("Can't delete node (Node not found)");
	_nodes.erase(it);
	delete node;
	return SUCCESS;
}


int SceneTree::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	for (auto it : _nodes) {
		it.second->shader->setupDraw(projectionMatrix, viewMatrix,
					it.second->modelMatrix, _lightPos);
		// SHOULD ADD ALL PARENTS TRANSFORMATIONS AS WELL!
		if (it.second->texture)
			it.second->texture->setupDraw(
				it.second->shader->getProgramID());
		it.second->obj->draw();
	}
	return SUCCESS;
}


Node *SceneTree::getNode(std::string nodeName)
{
	Node *node = NULL;
	auto it = _nodes.find(nodeName);
	if (it != _nodes.end()) {
		node = it->second;
	} else {
		printError("Can't give node (Node not found)");
		return NULL;
	}
	return node;
}


int SceneTree::setLightPos(glm::vec3 lightPos)
{
	_lightPos = lightPos;
	return SUCCESS;
}
