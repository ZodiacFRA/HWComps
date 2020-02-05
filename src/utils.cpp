#include "common_HWComps.hpp"

int printError(std::string msg)
{
	std::cout << C_RED << msg << C_RESET << std::endl;
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
