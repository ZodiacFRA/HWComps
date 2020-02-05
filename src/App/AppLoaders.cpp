#include "App.hpp"


int App::loadTextureLibrary()
{
	Texture *tmpTex = new Texture();
	tmpTex->loadTexture("./data/textures/Player.bmp", false);
	_textureLibrary.emplace("Player", tmpTex);

	tmpTex = new Texture();
	tmpTex->loadTexture("./data/textures/floor.bmp", false);
	_textureLibrary.emplace("floor", tmpTex);

	tmpTex = new Texture();
	tmpTex->loadTexture("./data/textures/cube.bmp", false);
	_textureLibrary.emplace("cube", tmpTex);

	tmpTex = new Texture();
	tmpTex->loadTexture("./data/textures/dev.bmp", false);
	_textureLibrary.emplace("dev", tmpTex);

	tmpTex = new Texture();
	tmpTex->loadTexture("./data/textures/wall.bmp", false);
	_textureLibrary.emplace("wall", tmpTex);
	
	return SUCCESS;
}


int App::loadObjsLibrary()
{
	Obj *tmpObj = new Obj("axis"); // Name of the mesh itself
	tmpObj->loadObj("./data/objs/axis.obj");
	_objsLibrary.emplace("axis", tmpObj);

	tmpObj = new Obj("cube");
	tmpObj->loadObj("./data/objs/cube.obj");
	_objsLibrary.emplace("cube", tmpObj);

	tmpObj = new Obj("Player");
	tmpObj->loadObj("./data/objs/Player.obj");
	_objsLibrary.emplace("Player", tmpObj);

	tmpObj = new Obj("floor");
	tmpObj->loadObj("./data/objs/floor.obj");
	_objsLibrary.emplace("floor", tmpObj);

	tmpObj = new Obj("wall");
	tmpObj->loadObj("./data/objs/wall.obj");
	_objsLibrary.emplace("wall", tmpObj);

	tmpObj = new Obj("malus");
	tmpObj->loadObj("./data/objs/malus.obj");
	_objsLibrary.emplace("malus", tmpObj);
	return SUCCESS;
}


int App::initShaders()
{
	Shader *tmpShader = new Shader();
	tmpShader->loadShaders("./data/shaders/StandardShading/vs.glsl",
				"./data/shaders/StandardShading/fs.glsl"
	);
	_shadersLibrary.emplace("StandardShading", tmpShader);


	tmpShader = new Shader();
	tmpShader->loadShaders("./data/shaders/colored/vs.glsl",
				"./data/shaders/colored/fs.glsl"
	);
	_shadersLibrary.emplace("colored", tmpShader);

	tmpShader = new Shader();
	tmpShader->loadShaders("./data/shaders/StandardShadingNoSpec/vs.glsl",
				"./data/shaders/StandardShadingNoSpec/fs.glsl"
	);
	_shadersLibrary.emplace("StandardShadingNoSpec", tmpShader);

	tmpShader = new Shader();
	tmpShader->loadShaders("./data/shaders/StandardShadingNoSpec/vs.glsl",
				"./data/shaders/ShaderToy/fs.glsl"
	);
	_shadersLibrary.emplace("ShaderToy", tmpShader);

	return SUCCESS;
}
