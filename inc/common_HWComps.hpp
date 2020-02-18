#pragma once

#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string.h> // for memcmp
#include <dirent.h>
#include <cstring>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <cmath>
#include <list>
#include <math.h>
#include <map>

#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/string_cast.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/norm.hpp>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#define FAILURE 0
#define SUCCESS 1

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

#define NOT_COLLIDING 0
#define COLLIDING 1
#define FLOOR_COLLIDING 2

#define PI 3.14159265

#define WIN_WIDTH 1920/2
#define WIN_HEIGHT 1052
#define WIN_NAME "HWGL"

#define MOUSE_H_MOV_SPEED 0.0005f
#define MOUSE_V_MOV_SPEED 0.0001f

#define C_RED "\x1b[31m"
#define C_GREEN "\x1b[32m"
#define C_BOLD_GREEN "\x1b[1;32m"
#define C_GREEN "\x1b[32m"
#define C_YELLOW "\x1b[33m"
#define C_BLUE "\x1b[36m"
#define C_RESET "\x1b[0m"

int printError(std::string msg);
int printWarning(std::string msg);
float getRandomFloat(float min, float max);
std::vector<std::string> splitString(std::string strToSplit, char delimeter);
std::vector<std::string> getDirFilenamesList(std::string dirPath);
