#pragma once

#include "common_HWComps.hpp"
#include "vboindexer.hpp"
#include "Shader.hpp"


class Obj {
public:
	Obj(std::string name);
	virtual ~Obj ();

	int loadObj(std::string filepath);
	int draw();

	std::string _name;
	glm::vec3 _mins;
	glm::vec3 _maxs;
private:
	bool _initDone = false;
	std::vector<glm::vec3> _vertices;
	std::vector<glm::vec2> _uvs;
	std::vector<glm::vec3> _normals;

	std::vector<unsigned short> _indices;
	std::vector<glm::vec3> _indexed_vertices;
	std::vector<glm::vec2> _indexed_uvs;
	std::vector<glm::vec3> _indexed_normals;

	GLuint _verticesBuffer;
	GLuint _uvBuffer;
	GLuint _normalBuffer;
	GLuint _elementBuffer;

	int loadFromObjFile(FILE *file);
	int drawBuffer(GLuint buffer, int attribute, int size);
};
