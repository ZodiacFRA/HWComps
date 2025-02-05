#pragma once

#include "common_HWComps.hpp"

class Shader {
public:
	GLuint loadShaders(const char *v_path, const char *f_path);
	virtual ~Shader();

	int setupDraw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix,
			glm::mat4 modelMatrix, glm::vec3 lightPos);

	// GETTERS
	GLuint getProgramID() { return _programID; }

	GLuint _programID;
private:

	GLuint _matrixID;
	GLuint _modelMatrixID;
	GLuint _viewMatrixID;

	// Will be used only if shader does have a "LightPosition_worldspace"
	GLuint _lightID;


	int loadShader(GLuint shaderID, const char *path);
	int initMatricesIDs();
};
