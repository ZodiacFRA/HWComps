#pragma once

#include "common_HWComps.hpp"
#include "Obj.hpp"
#include "Shader.hpp"
#include "SceneTree.hpp"
#include "Texture.hpp"

class App {
public:
	App();
	~App();

	int init();
	int run();

private:
	int loadConstantsFromFile(std::string fileName);

	int initGLFW();
	int initGLEW();
	int initVertexArray();
	int initTexture2D();

	int initShaders();
	int loadObjsLibrary();
	int loadTextureLibrary();


	int setupScene();
	int createMap();

	Node *createNode(std::string parentNodeName, std::string nodeName,
			std::string objName, std::string shaderName,
			std::string textureName, glm::vec3 position, int checkCollisions=1,
			int randomID=-1);

	Node *createNode(std::string parentNodeName, std::string nodeName,
			std::string objName, std::string shaderName,
			std::string textureName, glm::vec3 position, glm::vec3 rM,
			float rotationAngle, int checkCollisions=1, int randomID=1);

	int computeMatricesFromInputs();

	int handleTime();
	int handleAspectRatio();

	// Player
	int handlePlayerMovement();
	int handleJump();
	int handleMove();

	// Fonts
	void printText2D(const char * text, int x, int y, int size);
	void cleanupText2D();

	// Utils
	int _winWidth;
	int _winHeight;
	double _lastTime;
	double _currentTime;
	float _deltaTime;
	int _nbFrames;
	glm::vec3 _upVector;

	// GL
	GLFWwindow *_win;
	GLuint _vertexArrayID;

	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;

	// Camera
	float _radius;
    float _minCamHeight;
    float _maxCamHeight;
	float _camHeight;
    float _camHeightOffset;
	float _fov;

	glm::vec3 _camPos;
	double _xMouse;
	double _yMouse;

	std::map<std::string, Shader *> _shadersLibrary;
	std::map<std::string, Obj *> _objsLibrary;
	std::map<std::string, Texture *> _textureLibrary;
	SceneTree _sceneTree;

	// Player
	Node *_playerNode;
	float _jumpStart;
	float _yaw;
	float _pitch;
	float _roll;

	float _maxRollAngle;
	float _maxForwardAngle;
	float _maxBackwardAngle;

	float _playerForwardSpeed;
	float _playerBackwardSpeed;
	float _playerLateralSpeed;
	float _forcesReductionFactor;

	// Fonts
	unsigned int _text2DTextureID;
	unsigned int _text2DVertexBufferID;
	unsigned int _text2DUVBufferID;
	unsigned int _text2DShaderID;
	unsigned int _text2DUniformID;
	Texture _fontTexture;
	Shader _fontShader;
	float _fps;
};
