#include "App.hpp"

int App::transformPosforAStar(glm::vec3 pos)
{
	int idx = _pathfinder.toFlatArray(
						(abs(_mapMins.x) + pos.x) / _nodeRes,
						(abs(_mapMins.z) + pos.z) / _nodeRes);
	if (idx < 0 || idx > (int)_pathfinder._map.size())
		return FAILURE;
	if (!_pathfinder._map[idx]->_isFree) {
		int neighbor = _pathfinder.getNeighbor(idx);
		if (neighbor != -1)
			return neighbor;
	}
	return idx;
}

glm::vec3 App::transformPosforGlm(int x, int y)
{
	return glm::vec3(
				x * _nodeRes - abs(_mapMins.x),
				1,
				y * _nodeRes - abs(_mapMins.z));
}

void App::getSpawns(std::vector<glm::vec3> &spawnList)
{
	for (size_t i = 0; i < _mapAnalysis.size(); i++) {
		for (size_t j = 0; j < _mapAnalysis.size(); j++) {
			if (_mapAnalysis[i][j])
				spawnList.push_back(transformPosforGlm(i, j));
		}
	}
}

Node *App::createNode(std::string parentNodeName, std::string nodeName,
		std::string objName, std::string shaderName,
		std::string textureName, glm::vec3 position, int checkCollisions,
		int applyGravity, int randomID)
{
	Obj *tmpObj = NULL;
	auto it = _objsLibrary.find(objName);
	// Add .obj to the node
	if (it != _objsLibrary.end()) {
		tmpObj = it->second;
	} else {
		printError("Can't create node (Obj does not exist)");
		return NULL;
	}

	// Add shader to the node
	Shader *tmpShader = NULL;
	auto it1 = _shadersLibrary.find(shaderName);
	if (it1 != _shadersLibrary.end()) {
		tmpShader = it1->second;
	} else {
		printError("Fallback Shader (Shader does not exist)");
		tmpShader = _shadersLibrary.begin()->second;
	}

	// Add texture to the node
	Texture *tmpTexture = NULL;
	if (textureName != "") {
		auto it2 = _textureLibrary.find(textureName);
		if (it2 != _textureLibrary.end()) {
			tmpTexture = it2->second;
		} else {
			std::cout << textureName;
			printError(": not found, using Fallback Texture");
			tmpTexture = _textureLibrary.begin()->second;
		}
	}
	return _sceneTree.insert(parentNodeName, nodeName, tmpObj, tmpShader,
				tmpTexture, position, checkCollisions, applyGravity, randomID);
}

int App::handleAspectRatio()
{
	// Handles window resize, I successfully change the aspectRatio
	// but not the window limits, even when calling glfwSetWindowSizeLimits
	static int oldWinWidth = _winWidth;
	static int oldWinHeight = _winHeight;
	// Check if window as been resized
	glfwGetWindowSize(_win, &_winWidth, &_winHeight);
	if (_winWidth != oldWinWidth || _winHeight != oldWinHeight) {
		glfwSetWindowSizeLimits(_win, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE);
		glfwSetWindowAspectRatio(_win, GLFW_DONT_CARE, GLFW_DONT_CARE);
		oldWinWidth = _winWidth;
	}
	return SUCCESS;
}


int App::handleTime()
{
	_currentTime = glfwGetTime();
	static float lastFrameTime = _currentTime;

	_deltaTime = _currentTime - _lastTime;
	_lastTime = _currentTime;

	// FPS counter
	_nbFrames++;
	if (_nbFrames >= 9) {
		_fps = float(_nbFrames) / 10 * (1 / (_currentTime - lastFrameTime));
		_nbFrames = 0;
		lastFrameTime = _currentTime;
		return true;
	}
	return false;
}


int App::loadConstantsFromFile(std::string fileName)
{
	std::ifstream in(fileName.c_str());
	if (!in)
		return printError("Cannot open physics file");

	std::string str;
	// Read the next line from File untill it reaches the end.
	while (std::getline(in, str)) {
		if (str.size() <= 0)
			continue;
		std::vector<std::string> splitted = splitString(str, '/');
		if (splitted[0] == "_radius")
			_radius = std::stof(splitted[1], NULL);
		if (splitted[0] == "_minCamHeight")
			_minCamHeight = std::stof(splitted[1], NULL);
		if (splitted[0] == "_maxCamHeight")
			_maxCamHeight = std::stof(splitted[1], NULL);
		if (splitted[0] == "_camHeightOffset")
			_camHeightOffset = std::stof(splitted[1], NULL);
		if (splitted[0] == "_fov")
			_fov = std::stof(splitted[1], NULL);

		if (splitted[0] == "_yaw")
			_yaw = std::stof(splitted[1], NULL);
		if (splitted[0] == "_pitch")
			_pitch = std::stof(splitted[1], NULL);
		if (splitted[0] == "_roll")
			_roll = std::stof(splitted[1], NULL);

		if (splitted[0] == "_maxRollAngle")
			_maxRollAngle = std::stof(splitted[1], NULL);
		if (splitted[0] == "_maxForwardAngle")
			_maxForwardAngle = std::stof(splitted[1], NULL);
		if (splitted[0] == "_maxBackwardAngle")
			_maxBackwardAngle = std::stof(splitted[1], NULL);

		if (splitted[0] == "_playerForwardSpeed")
			_playerForwardSpeed = std::stof(splitted[1], NULL);
		if (splitted[0] == "_playerBackwardSpeed")
			_playerBackwardSpeed = std::stof(splitted[1], NULL);
		if (splitted[0] == "_playerLateralSpeed")
			_playerLateralSpeed = std::stof(splitted[1], NULL);

		if (splitted[0] == "_gravityValue")
			_gravityValue = std::stof(splitted[1], NULL);
		if (splitted[0] == "_jumpFactor")
			_jumpFactor = std::stof(splitted[1], NULL);
		if (splitted[0] == "_jumpMaxLength")
			_jumpMaxLength = std::stof(splitted[1], NULL);

		if (splitted[0] == "_scanResolution")
			_scanResolution = std::stof(splitted[1], NULL);
	}
	in.close();
	_camHeight = (_minCamHeight + _maxCamHeight) / 2;
	float maxFrameForce = sqrt(pow(_playerForwardSpeed, 2) + pow(_playerLateralSpeed, 2));
	_forcesReductionFactor = _playerForwardSpeed / maxFrameForce;
	return SUCCESS;
}
