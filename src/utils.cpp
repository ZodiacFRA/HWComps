#include "common_HWComps.hpp"


int printError(std::string msg)
{
	std::cout << C_RED << msg << C_RESET << std::endl;
	return FAILURE;
}

int printWarning(std::string msg)
{
	std::cout << C_YELLOW << msg << C_RESET << std::endl;
	return FAILURE;
}


auto &getGen()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return gen;
}


float getRandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> randFloat(min, max);
        return randFloat(getGen());
}


std::vector<std::string> splitString(std::string strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
       splittedStrings.push_back(item);
    }
    return splittedStrings;
}


std::vector<std::string> getDirFilenamesList(std::string dirPath)
{
	DIR *dir;
	struct dirent *ent;
	std::vector<std::string> res;

	if ((dir = opendir(dirPath.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if ((int)ent->d_type == 8)
				res.push_back(ent->d_name);
		}
		closedir(dir);
	} else {
		perror ("");
		return res;
	}
	return res;
}
