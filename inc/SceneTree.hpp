#pragma once

#include "common_HWComps.hpp"
#include "Obj.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Particles.hpp"


struct Node;
struct Node {
	std::string name;
	glm::mat4 modelMatrix;
	int randomID;  // Used for the random animation of the props

	int checkCollisions = true;
	int applyGravity = false;

	Node *parent;
	std::vector<Node *> childs;

	Obj *obj;
	Shader *shader;
	Texture *texture;
};


class SceneTree {
// Not implementing a real tree algorithm for now.
// I'll traverse the graph only to get all parents transformation
// matrices before drawing an object.
// If later a lot of depth is used I'll implement a proper
// graph traversal algorithm.
public:
	SceneTree();
	virtual ~SceneTree();

	Node *insert(std::string parentName, std::string name, Obj *obj,
			Shader *shader, Texture *texture, glm::vec3 position,
			int checkCollisions, int applyGravity, int randomID);

	int remove(std::string nodeName);

	Node *getNode(std::string nodeName);
	int setLightPos(glm::vec3 lightPos);

	int draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

	int isColliding(Node *n1, glm::vec3 nextPos, Node *n2);
	int applyGravity(float gravityValue);

	int translateNode(std::string nodeName, glm::vec3 tM);
	int translateNode(Node *node, glm::vec3 tM, bool onlyCheckCollision=false);

	int setNodePosition(std::string nodeName, glm::vec3 pM);
	int rotateNode(std::string nodeName, float degrees, glm::vec3 rM);
	int rotateNodeRad(std::string nodeName, float rads, glm::vec3 rM);
	int scaleNode(std::string nodeName, glm::vec3 sM);
	int setNodeScale(std::string nodeName, glm::vec3 sM);
	// DOES NOT WORK
	int setNodeRotation(std::string nodeName, float degrees, glm::vec3 rM);

	int handleParticles(float deltaTime, glm::vec3 cameraPosition, float yaw, float pitch);

	std::map<std::string, Node *> _nodes;

	Particles _particles;
private:
	Node _root;
	glm::vec3 _lightPos;
};
