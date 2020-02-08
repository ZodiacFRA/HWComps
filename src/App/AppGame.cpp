#include "App.hpp"


int App::createMap()
{
	// 1 openGL unit = 1m in Blender
	// parentNodeName, Node name, Obj name, Shader name,
	// 		Texture name, Position, (rM, rot_angle, ID)
	for (auto it: _objsLibrary) {
		if (it.first != "Player") {
			createNode(
					"", it.first, it.first, "StandardShading",
					it.first, glm::vec3(0, 0, 0)
			);
		}
	}

	return SUCCESS;
}
