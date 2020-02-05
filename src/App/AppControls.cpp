#include "App.hpp"


int App::computeMatricesFromInputs()
{
	glm::vec3 playerPosMatrix = _playerNode->modelMatrix[3];
	// Get mouse position
	glfwGetCursorPos(_win, &_xMouse, &_yMouse);
	// Reset mouse position for next frame
	glfwSetCursorPos(_win, _winWidth / 2, _winHeight / 2);
	// Compute new orientation
	_hAngle += MOUSE_MOV_SPEED * float(_winWidth / 2 - _xMouse);
	_vAngle += MOUSE_MOV_SPEED * float(_winHeight / 2 - _yMouse);

	// fov, ratio, display range
	_projectionMatrix = glm::perspective(
				glm::radians(_fov),
				float(_winWidth) / float(_winHeight),
				0.01f, 5000.0f);

	_viewMatrix = glm::lookAt(
			_camPos,
			glm::vec3(
				playerPosMatrix[0],
				playerPosMatrix[1] + _camHeightOffset,
				playerPosMatrix[2]),
			glm::vec3(0, 1, 0));  // Up vector
	return SUCCESS;
}
