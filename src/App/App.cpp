#include "App.hpp"


App::App()
	: _winWidth(WIN_WIDTH), _winHeight(WIN_HEIGHT),  // store win size to handle resize
	_lastTime(glfwGetTime()),
	_camPos(glm::vec3(0, 15, -10)),  // RIGHT / HEIGHT / FRONT
	_jumpStart(glfwGetTime()),
	_lastAStarTime(glfwGetTime())
{
	srand(time(NULL));
	// Is also called in AppPlayer.cpp handleMove() for hot reload
	loadConstantsFromFile("./data/physics.txt");
}

int App::run()
{
	do {
		if (handleTime()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			handlePlayerMovement();
			moveNPCs();
			 _sceneTree.applyGravity(_gravityValue);

			if (!computeMatricesFromInputs())
				return FAILURE;

			_sceneTree.handleParticles(_deltaTime, _camPos, _yaw);
			_sceneTree.draw(_projectionMatrix, _viewMatrix);

			// Draw info text
			char text[256];
			sprintf(text,"FPS: %f", _fps);
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
	for (auto &it : _shadersLibrary) {
		if (it.second)
			delete it.second;
	}
	for (auto &it : _objsLibrary) {
		if (it.second)
			delete it.second;
	}
	for (auto &it : _textureLibrary) {
		if (it.second)
			delete it.second;
	}
	// Properly delete all GL attributes and buffers
	glDeleteVertexArrays(1, &_vertexArrayID);
	cleanupText2D();
	glfwTerminate();
}
