#include "App.hpp"


int App::createMap()
{
	// 1 openGL unit = 1m in Blender
	// parentNodeName, Node name, Obj name, Shader name,
	// 		Texture name, Position, (rM, rot_angle, ID)
	for (auto &it: _objsLibrary) {
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
	if (_currentTime - _lastAStarTime > 0.1) {
		int from = transformPosforAStar(_NPCs[_lastNpcDone]._node->modelMatrix[3]);
		int to = transformPosforAStar(glm::vec3(
							_playerNode->modelMatrix[3].x + _NPCs[_lastNpcDone]._targetPosDelta.x,
							_playerNode->modelMatrix[3].y + _NPCs[_lastNpcDone]._targetPosDelta.y,
							_playerNode->modelMatrix[3].z + _NPCs[_lastNpcDone]._targetPosDelta.z
						));
		std::vector<std::pair<int, int>> newPathAstar = _pathfinder.compute(from, to);
		std::list<glm::vec3> newPathGlm;
		for (auto &tmpPair : newPathAstar)
			newPathGlm.push_back(transformPosforGlm(tmpPair.first, tmpPair.second));
		_NPCs[_lastNpcDone].updatePath(newPathGlm);
		_lastAStarTime = _currentTime;

		_lastNpcDone += 1;
		if (_lastNpcDone >= _NPCs.size())
			_lastNpcDone = 0;
	}
	for (size_t i = 0; i < _NPCs.size(); i++) {
		float totalTurnDistance = 0.0;
		while (totalTurnDistance >= 0 && totalTurnDistance < _NPCs[i]._speed) {
			// printf("Moving NPC %d\n", i);
			_sceneTree.translateNode(_NPCs[i]._node, _NPCs[i].getNextMove(totalTurnDistance));
		}
	}
	return SUCCESS;
}
