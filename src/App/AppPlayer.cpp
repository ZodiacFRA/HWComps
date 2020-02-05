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
	// Get mouse position
	glfwGetCursorPos(_win, &_xMouse, &_yMouse);
	// Reset mouse position for next frame
	glfwSetCursorPos(_win, _winWidth / 2, _winHeight / 2);

	float delta = MOUSE_H_MOV_SPEED * float(_winWidth / 2 - _xMouse);
	_yaw += delta;
	if (_yaw >= 2 * PI)
		_yaw -= 2 * PI;
	else if (_yaw <= 0)
		_yaw += 2 * PI;

	_sceneTree.rotateNodeRad("PlayerNode", delta, glm::vec3(0, 1, 0));
	glm::vec3 movement = glm::vec3(0, 0, 0);

	if (glfwGetKey(_win, GLFW_KEY_ENTER) == GLFW_PRESS)
		loadConstantsFromFile("./data/physics.txt");

	float roll_d = 0.0;
	float pitch_d = 0.0;
	float rot_speed = 0.1;

	if (glfwGetKey(_win, GLFW_KEY_A) == GLFW_PRESS) {
		movement[0] += _playerLateralSpeed;
		if (_roll > -_maxRollAngle)
			roll_d -= rot_speed;
	} else if (glfwGetKey(_win, GLFW_KEY_D) == GLFW_PRESS) {
		movement[0] -= _playerLateralSpeed;
		if (_roll < _maxRollAngle)
			roll_d += rot_speed;
	} else {
		roll_d = -_roll / 2;
	}
	if (glfwGetKey(_win, GLFW_KEY_W) == GLFW_PRESS) {
		movement[2] += _playerForwardSpeed;
		if (_pitch < _maxForwardAngle)
			pitch_d += rot_speed;
	} else if (glfwGetKey(_win, GLFW_KEY_S) == GLFW_PRESS) {
		movement[2] -= _playerBackwardSpeed;
		if (_pitch > -_maxBackwardAngle)
			pitch_d -= rot_speed;
	} else {
		pitch_d = -_pitch / 2;
	}

	if (movement[0] != 0 && movement[2] != 0) {
		movement[0] *= _forcesReductionFactor;
		movement[2] *= _forcesReductionFactor;
	}
	_sceneTree.translateNode("PlayerNode", movement);
	// glm::vec3 ppmN = _playerNode->modelMatrix[3];

	// _sceneTree.rotateNodeRad("PlayerNode", roll_d, glm::vec3(0, 0, 1));
	// _sceneTree.rotateNodeRad("PlayerNode", pitch_d, glm::vec3(1, 0, 0));

	_roll += roll_d;
	_pitch += pitch_d;
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
