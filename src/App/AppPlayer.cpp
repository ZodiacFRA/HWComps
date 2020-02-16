#include "App.hpp"


int App::handlePlayerMovement()
{
	handleMove();
	handleJump();

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

	float tmpGravity = _gravityValue;

	if (!_playerIsOnGround) {
		movement[0] *= 0.7;
		movement[2] *= 0.7;
	}

	if (movement[0] != 0 && movement[2] != 0) {
		movement[0] *= _forcesReductionFactor;
		movement[2] *= _forcesReductionFactor;
		tmpGravity *= 0.7;
	}

	_sceneTree.translateNode(_playerNode, movement);

	if (_sceneTree.translateNode(_playerNode, glm::vec3(0, _gravityValue, 0)) == 2) {
		_playerIsOnGround = true;
		_jumpStart = 0.0;
	} else {
		_playerIsOnGround = false;
	}
	// glm::vec3 ppmN = _playerNode->modelMatrix[3];
	// std::cout << ppmN[1] << '\n';

	// _sceneTree.rotateNodeRad("PlayerNode", roll_d, glm::vec3(0, 0, 1));
	// _sceneTree.rotateNodeRad("PlayerNode", pitch_d, glm::vec3(1, 0, 0));

	_roll += roll_d;
	_pitch += pitch_d;

	if (glfwGetKey(_win, GLFW_KEY_U) == GLFW_PRESS) {
		int from = transformPosforAStar(glm::vec3(0, 1, 0));
		int to = transformPosforAStar(_playerNode->modelMatrix[3]);
		std::vector<std::pair<int, int>> res = _pathfinder.compute(
				transformPosforAStar(glm::vec3(0, 1, 0)),
				transformPosforAStar(_playerNode->modelMatrix[3])
		);
	}

	return SUCCESS;
}

int App::handleJump()
{
	if (glfwGetKey(_win, GLFW_KEY_SPACE) == GLFW_PRESS && _playerIsOnGround) {
		_jumpStart = _currentTime;
		applyJump();
	} else if (!_playerIsOnGround) {
		applyJump();
	}
	return SUCCESS;
}

int App::applyJump()
{
	float deltaT = _currentTime - _jumpStart;
	if (deltaT > _jumpMaxLength)
		return SUCCESS;
	if (deltaT < 0)
		deltaT = 0.0;
	// float newX = 20 * deltaT + 0.4;
	// float force = _jumpFactor * newX / (newX * newX + 1);
	float force = _jumpFactor * -log(deltaT);

	if (force > 0) {
		if (force > 2.5)
			force = 2.5;
		glm::vec3 jumpForce = glm::vec3(0, force, 0);
		_sceneTree.translateNode("PlayerNode", jumpForce);
	}
	return SUCCESS;
}
