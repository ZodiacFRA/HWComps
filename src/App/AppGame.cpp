#include "App.hpp"


int App::createMap()
{
	// 1 openGL unit = 1m in Blender
	// parentNodeName, Node name, Obj name, Shader name,
	// 		Texture name, Position, (rM, rot_angle, ID)
	createNode("", "floor", "floor", "StandardShading",
		"floor", glm::vec3(0, 0, 0));

	createNode("", "borderWall1", "wall1", "StandardShading",
		"wall", glm::vec3(0, 0, 0));

	createNode("", "borderWall2", "wall2", "StandardShading",
		"wall", glm::vec3(0, 0, 0));

	createNode("", "borderWall3", "wall3", "StandardShading",
		"wall", glm::vec3(0, 0, 0));

	createNode("", "borderWall4", "wall4", "StandardShading",
		"wall", glm::vec3(0, 0, 0));

	createNode("", "cubetest", "cube", "StandardShading",
		"cube", glm::vec3(3, 1, 3));
	return SUCCESS;
}
