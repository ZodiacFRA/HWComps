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


int App::moveNPCs()
{
	if (_currentTime - _lastAStarTime > 1.0) {
		_lastNpcDone += 1;
		if (_lastNpcDone >= _NPCs.size())
			_lastNpcDone = 0;
		int from = transformPosforAStar(_NPCs[_lastNpcDone]._node->modelMatrix[3]);
		int to = transformPosforAStar(_playerNode->modelMatrix[3]);
		std::vector<std::pair<int, int>> newPathAstar = _pathfinder.compute(from, to);
		std::vector<glm::vec3> newPathGlm;
		for (auto tmpPair : newPathAstar)
			newPathGlm.push_back(transformPosforGlm(tmpPair.first, tmpPair.second));
		_NPCs[_lastNpcDone].updatePath(newPathGlm);
		_lastAStarTime = _currentTime;
	}
	for (auto npc : _NPCs)
		_sceneTree.translateNode(npc._node, npc.getNextMove());
	return SUCCESS;
}
