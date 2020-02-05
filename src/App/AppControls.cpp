#include "App.hpp"


int App::computeMatricesFromInputs()
{
	glm::vec3 ppM = _playerNode->modelMatrix[3];
	_camPos = glm::vec3(
				ppM[0] + ((_radius * cos(_camHeight)) * sin(_yaw)),
				ppM[1] + (_radius * sin(_camHeight)),
				ppM[2] + ((_radius * cos(_camHeight)) * cos(_yaw))
	);

	// _yMouse is already updated in handle_move() (AppPlayer.cpp)
	_camHeight += (MOUSE_V_MOV_SPEED * float(_winHeight / 2 - _yMouse));
	if (_camHeight > _maxCamHeight)
		_camHeight = _maxCamHeight;
	else if (_camHeight < _minCamHeight)
		_camHeight = _minCamHeight;

	// fov, ratio, display range
	_projectionMatrix = glm::perspective(
				glm::radians(_fov),
				float(_winWidth) / float(_winHeight),
				0.01f, 5000.0f);

	_viewMatrix = glm::lookAt(
			_camPos,
			glm::vec3(
				ppM[0],
				ppM[1] + _camHeightOffset,
				ppM[2]),
			glm::vec3(0, 1, 0));  // Up vector
	return SUCCESS;
}
