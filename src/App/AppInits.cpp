#include "App.hpp"


int App::init()
{
	// Master initialization function
	int flag = SUCCESS;
	flag &= initGLFW();
	flag &= initGLEW();
	flag &= initVertexArray();
	flag &= initTexture2D();

	flag &= initShaders();
	flag &= loadObjsLibrary();
	flag &= loadTextureLibrary();
	flag &= _sceneTree._particles.init(
									_shadersLibrary["Particles"],
									_textureLibrary["particle"]
				);

	flag &= setupScene();
	flag &= _sceneTree._particles.startParticleSystem();
	return flag;
}

int App::setupScene()
{

	createMap();
	analyzeMap();
	_pathfinder.initMap(_mapAnalysis);
	// 1 openGL unit = 1m in Blender
	// parentNodeName, Node name, Obj name, Shader name, Texture name, Position
	_playerNode = createNode("", "PlayerNode", "Player", "StandardShading",
		"Player", glm::vec3(0, 0, 0), 1, 1, -1);
	for (int i = 0; i < 2; i++) {
		Node *tmpNode = createNode("", "NPC_" + std::to_string(i), "Player",
			"StandardShading", "floor", glm::vec3(3 * i + 1, 1, 3 * i + 1), 1, 1, -1);
		NPC tmpNpc;
		tmpNpc._node = tmpNode;
		_NPCs.push_back(tmpNpc);
	}
	return SUCCESS;
}


int App::initTexture2D()
{
	// Use https://www.codehead.co.uk/cbfg/ to create bmp from font
	// then mirror it upside down (no joke) with gimp
	_text2DTextureID = _fontTexture.loadTexture("./data/fonts/Iosevka.bmp", false);
	// Initialize VBO
	glGenBuffers(1, &_text2DVertexBufferID);
	glGenBuffers(1, &_text2DUVBufferID);
	// Initialize Shader
	_text2DShaderID = _fontShader.loadShaders("./data/shaders/TextShader/TextVertexShader.vertexshader",
				"./data/shaders/TextShader/TextVertexShader.fragmentshader" );
	// Initialize uniforms IDs
	_text2DUniformID = glGetUniformLocation(_text2DShaderID, "myTextureSampler");
	return SUCCESS;
}


int App::initGLFW()
{
	if (!glfwInit())
		return printError("Failed to initialize GLFW");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	_win = glfwCreateWindow(_winWidth, _winHeight, WIN_NAME, NULL, NULL);
	if (_win == NULL)
		return printError("Failed to open GLFW window");
	glfwMakeContextCurrent(_win);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(_win, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(_win, _winWidth/2, _winHeight/2);

	// Set background color
	glClearColor(0.2, 0.2, 0.3, 0.0);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	return SUCCESS;
}


int App::initGLEW()
{
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
		return printError("Failed to initialize GLEW");
	return SUCCESS;
}


int App::initVertexArray()
{
	glGenVertexArrays(1, &_vertexArrayID);
	glBindVertexArray(_vertexArrayID);
	return SUCCESS;
}
