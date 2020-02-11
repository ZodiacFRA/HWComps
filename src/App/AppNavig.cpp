#include "App.hpp"


int App::analyzeMap()
{
    printf("%s-------------------------------------\nStarting map analysis%s\n", C_BOLD_GREEN, C_RESET);
    glm::vec3 mapMaxs = glm::vec3(-999999999, -999999999, -999999999);
    glm::vec3 mapMins = glm::vec3(999999999, 999999999, 999999999);
    Node *minYNode;  // Floor
    for (auto node : _sceneTree._nodes) {
            // printf("node name: %s, node min: %f, mapMin: %f\n", node.first, node.second->obj->_mins[i])
            mapMins.x = std::min(mapMins.x, node.second->obj->_mins.x);
            mapMaxs.x = std::max(mapMaxs.x, node.second->obj->_maxs.x);

            if (node.second->obj->_mins.y < mapMins.y) {
                mapMins.y = node.second->obj->_mins.y;
                minYNode = node.second;
            }
            mapMaxs.y = std::max(mapMaxs.y, node.second->obj->_maxs.y);

            mapMins.z = std::min(mapMins.z, node.second->obj->_mins.z);
            mapMaxs.z = std::max(mapMaxs.z, node.second->obj->_maxs.z);
    }
    printf("Detected Map boundaries are:\n");
    float mapXSize = mapMaxs.x - mapMins.x;
    float mapYSize = mapMaxs.y - mapMins.y;
    float mapZSize = mapMaxs.z - mapMins.z;
    float floorHeight = minYNode->obj->_maxs.y;
    printf("\tX: %f <-> %f\t-> %f\n", mapMins.x, mapMaxs.x, mapXSize);
    printf("\tY: %f <-> %f\t-> %f\n", mapMins.y, mapMaxs.y, mapYSize);
    printf("\tZ: %f <-> %f\t-> %f\n", mapMins.z, mapMaxs.z, mapZSize);
    printf("\tFloor level detected at y= %f for node %s\n", floorHeight, minYNode->name.c_str());
    float nodeRes = std::max(mapXSize / _scanResolution, mapZSize / _scanResolution);
    printf("A* scan resolution size = %f units\n", nodeRes);
    // Create scanning node and object for collision detection
    Node *collisionNode = new Node();
    collisionNode->name = "collisionNode";
    collisionNode->obj = new Obj("collisionNode");
    collisionNode->modelMatrix = glm::mat4(1.0f);
    collisionNode->obj->_mins = glm::vec3(-nodeRes / 2, 0.0, -nodeRes / 2);
    collisionNode->obj->_maxs = glm::vec3(nodeRes / 2, 2.0, nodeRes / 2);
    collisionNode->checkCollisions = false;
    _sceneTree.translateNode(collisionNode, glm::vec3(mapMins.x + nodeRes / 2, 0.2, mapMins.z + nodeRes / 2));
    printf("Starting at position %f, %f, %f\n",
                collisionNode->modelMatrix[3].x,
                collisionNode->modelMatrix[3].y,
                collisionNode->modelMatrix[3].z);
    collisionNode->checkCollisions = true;


    scanMap(collisionNode, nodeRes, mapMins);
    delete collisionNode->obj;
    delete collisionNode;
    return SUCCESS;
}

int App::scanMap(Node *collisionNode, float scanRes, glm::vec3 mapMins)
{
    for (int z = 0; z < _scanResolution; z++) {
        std::vector<bool> tmpX;
        for (int x = 0; x < _scanResolution; x++)
            tmpX.push_back(false);
        _mapAnalysis.push_back(tmpX);
    }
    for (int z = 0; z < _scanResolution; z++) {
        float zPos = scanRes * z;
        for (int x = 0; x < _scanResolution; x++) {
            float xPos = scanRes * x;
            int flag = _sceneTree.translateNode(collisionNode, glm::vec3(xPos, 0, zPos), true);
            if (flag == NOT_COLLIDING)  // No collision
                _mapAnalysis[z][x] = true;
        }
        std::cout << "1 rangée!" << '\n';
    }
    for (int z = 0; z < _scanResolution; z++) {
        for (int x = 0; x < _scanResolution; x++) {
            std::cout << _mapAnalysis[z][x];
        }
        std::cout << '\n';
    }
    return SUCCESS;
}
