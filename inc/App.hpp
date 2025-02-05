#pragma once

#include "common_HWComps.hpp"
#include "Obj.hpp"
#include "Shader.hpp"
#include "SceneTree.hpp"
#include "Texture.hpp"
#include "aStar.hpp"
#include "NPC.hpp"


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
			int applyGravity=0, int randomID=-1);

	int computeMatricesFromInputs();

	int handleTime();
	int handleAspectRatio();

	// Player
	int handlePlayerMovement();
	int handleJump();
	int applyJump();
	int handleMove();

	// Map + Navig
	int analyzeMap();
	int transformPosforAStar(glm::vec3 pos);
	glm::vec3 transformPosforGlm(int x, int y);
	void getSpawns(std::vector<glm::vec3> &spawnList);
	int scanMap(Node *collisionNode, float scanRes);

	int moveNPCs();

	// Fonts
	void printText2D(const char * text, int x, int y, int size);
	void cleanupText2D();

	// Utils
	int _winWidth;
	int _winHeight;
	double _lastTime;
	double _currentTime;
	float _deltaTime;
	int _nbFrames = 0;
	glm::vec3 _upVector = glm::vec3(0, 1, 0);

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
	float _jumpMaxLength;
	bool _playerIsOnGround = false;
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

	float _jumpFactor;
	float _gravityValue;

	// Map
	// Z first
	aStar _pathfinder;
	float _nodeRes;
	glm::vec3 _mapMaxs = glm::vec3(-999999999, -999999999, -999999999);
	glm::vec3 _mapMins = glm::vec3(999999999, 999999999, 999999999);
	int _scanResolution;
	std::vector<std::vector<bool>> _mapAnalysis;

	float _lastAStarTime;
	size_t _lastNpcDone = 0;
	std::vector<NPC> _NPCs;

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
