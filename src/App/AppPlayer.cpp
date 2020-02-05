#include "App.hpp"


int App::handlePlayerMovement()
{
	handleJump();
	handleMove();
	// -1 / (1 + exp(x - 4)) + 1
	return SUCCESS;
}

int App::handleMove()
{
	glm::vec3 movement = glm::vec3(0, 0, 0);

	bool goLeft = glfwGetKey(_win, GLFW_KEY_LEFT) == GLFW_PRESS;
	bool goRight = glfwGetKey(_win, GLFW_KEY_RIGHT) == GLFW_PRESS;
	bool goForward = glfwGetKey(_win, GLFW_KEY_UP) == GLFW_PRESS;
	bool goBackward = glfwGetKey(_win, GLFW_KEY_DOWN) == GLFW_PRESS;

	if (goLeft) {
		movement[0] += _playerLateralSpeed;
	} else if (goRight) {
		movement[0] -= _playerLateralSpeed;
	}
	if (goForward) {
		movement[2] += _playerForwardSpeed;
	} else if (goBackward) {
		movement[2] -= _playerBackwardSpeed;
	}

	_sceneTree.translateNode("PlayerNode", movement);
	return SUCCESS;
}

int App::handleJump()
{
	glm::vec3 playerPosMatrix = _playerNode->modelMatrix[3];

	bool jumpFlag = glfwGetKey(_win, GLFW_KEY_SPACE) == GLFW_PRESS;
	if (playerPosMatrix[1] < 0.1) {  // Can jump
		if (jumpFlag) {
			_jumpStart = _currentTime;
			_sceneTree.setNodePosition("PlayerNode",
				glm::vec3(playerPosMatrix[0], 0.1, playerPosMatrix[2]));
		}
	} else {  // Already in a jump
		float deltaT = _currentTime - _jumpStart;
		float height = 1.5 * sin(12 * deltaT + ((3.3 * PI) / 2)) + 1.5;
		_sceneTree.setNodePosition("PlayerNode",
				glm::vec3(playerPosMatrix[0], height, playerPosMatrix[2]));
	}
	return SUCCESS;
}
