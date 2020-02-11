#pragma once

#include "common_HWComps.hpp"


class Texture {
public:
	~Texture();
	int setupDraw(GLuint programID);
	GLuint loadTexture(const char *t_path, bool enableFiltering);
	GLuint loadDDS(const char *imagepath);

	GLuint _texture;
};
