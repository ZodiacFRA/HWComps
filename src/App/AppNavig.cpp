#include "App.hpp"


int App::computeMapBoundaries()
{
    glm::vec3 mapMaxs;
    glm::vec3 mapMins;

    for (int i = 0; i < 3; i++) {
        mapMaxs[i] = -999999999;
        mapMins[i] = 999999999;
    }
    for (auto node : _sceneTree._nodes) {
        for (int i = 0; i < 3; i++) {
            mapMins[i] = std::min(mapMins[i], node.second->obj->_mins[i]);
            mapMaxs[i] = std::max(mapMaxs[i], node.second->obj->_maxs[i]);
        }
    }
    printf("Detected Map boundaries are:\n");
    float mapXSize = mapMaxs.x - mapMins.x;
    float mapYSize = mapMaxs.y - mapMins.y;
    float mapZSize = mapMaxs.z - mapMins.z;
    printf("\tX: %f <-> %f\t-> %f\n", mapMins.x, mapMaxs.x, mapXSize);
    printf("\tY: %f <-> %f\t-> %f\n", mapMins.y, mapMaxs.y, mapYSize);
    printf("\tZ: %f <-> %f\t-> %f\n", mapMins.z, mapMaxs.z, mapZSize);
    float nodeXRes = mapXSize / 50;
    float nodeZRes = mapZSize / 50;
    printf("A* resolution size will be x= %f and z= %f\n", nodeXRes, nodeZRes);
    return SUCCESS;
}
