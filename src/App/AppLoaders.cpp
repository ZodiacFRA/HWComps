#include "App.hpp"


int App::loadTextureLibrary()
{
	std::vector<std::string> files = getDirFilenamesList("./data/textures");
	Texture *tmpTex;
	std::string path = "./data/textures/";
	for (auto file : files) {
		std::string tmp = path + file;
		std::vector<std::string> splitted = splitString(file, '.');
		tmpTex = new Texture();
		if (splitted[1] == "DDS") {
			tmpTex->loadDDS(tmp.c_str());
		} else {
			tmpTex->loadTexture(tmp.c_str(), false);
		}
		_textureLibrary.emplace(splitted[0], tmpTex);
	}
	return SUCCESS;
}


int App::loadObjsLibrary()
{
	std::vector<std::string> files = getDirFilenamesList("./data/objs");
	Obj *tmpObj;
	std::string path = "./data/objs/";
	for (auto file : files) {
		std::string tmp = path + file;
		std::vector<std::string> splitted = splitString(file, '.');
		tmpObj = new Obj(splitted[0]);
		tmpObj->loadObj(tmp.c_str());
		_objsLibrary.emplace(splitted[0], tmpObj);
	}
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
	tmpShader->loadShaders("./data/shaders/Particles/Particle.vertexshader",
				"./data/shaders/Particles/Particle.fragmentshader"
	);
	_shadersLibrary.emplace("Particles", tmpShader);

	return SUCCESS;
}
