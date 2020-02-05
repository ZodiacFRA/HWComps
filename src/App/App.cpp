#include "App.hpp"


App::App()
	: _winWidth(WIN_WIDTH), _winHeight(WIN_HEIGHT),  // store win size to handle resize
	_lastTime(glfwGetTime()), _nbFrames(0),  // speed handling
	// _radius(10.0), _minCamHeight(2.0), _maxCamHeight(3.0), // inverted angle?
	// _camHeight((_minCamHeight + _maxCamHeight) / 2),
	// _camHeightOffset(1.0), _fov(100.0),
	_camPos(glm::vec3(0, 15, -10)),  // RIGHT / HEIGHT / FRONT
	_jumpStart(0.0)
	// _yaw(0.0), _playerForwardSpeed(0.7),
	// _playerBackwardSpeed(0.3), _playerLateralSpeed(0.5)
{
	loadConstantsFromFile("./data/physics.txt");
	// Is also called in AppPlayer.cpp handleMove() for hot reload
}


int App::setupScene()
{
	// 1 openGL unit = 1m in Blender
	// parentNodeName, Node name, Obj name, Shader name, Texture name, Position
	_playerNode = createNode("", "PlayerNode", "Player", "StandardShading",
		"Player", glm::vec3(0, 0, 0));

	createNode("", "terrainMidNode", "floor", "StandardShading",
		"floor", glm::vec3(0, -1, 0));
	return SUCCESS;
}


int App::run()
{
	do {
		if (handleTime()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			handlePlayerMovement();

			if (!this->computeMatricesFromInputs())
				return FAILURE;

			_sceneTree.draw(_projectionMatrix, _viewMatrix);

			// Draw info text
			char text[256];
			sprintf(text,"FPS: %f", _camHeight);
			printText2D(text, 0, 570, 20);

			// Swap buffers
			glfwSwapBuffers(_win);
			glfwPollEvents();
		}
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(_win, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		!glfwWindowShouldClose(_win));
	return SUCCESS;
}


App::~App()
{
	// Properly delete all assets libraries
	for (auto it : _shadersLibrary) {
		if (it.second)
			delete it.second;
	}
	for (auto it : _objsLibrary) {
		if (it.second)
			delete it.second;
	}
	for (auto it : _textureLibrary) {
		if (it.second)
			delete it.second;
	}
	// Properly delete all GL attributes and buffers
	glDeleteVertexArrays(1, &_vertexArrayID);
	cleanupText2D();
	glfwTerminate();
}
