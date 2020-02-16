#include "App.hpp"


int App::analyzeMap()
{
    printf("%s-------------------------------------\nStarting map analysis%s\n", C_BOLD_GREEN, C_RESET);
    Node *minYNode;  // Floor
    for (auto node : _sceneTree._nodes) {
            // printf("node name: %s, node min: %f, mapMin: %f\n", node.first, node.second->obj->_mins[i])
            _mapMins.x = std::min(_mapMins.x, node.second->obj->_mins.x);
            _mapMaxs.x = std::max(_mapMaxs.x, node.second->obj->_maxs.x);

            if (node.second->obj->_mins.y < _mapMins.y) {
                _mapMins.y = node.second->obj->_mins.y;
                minYNode = node.second;
            }
            _mapMaxs.y = std::max(_mapMaxs.y, node.second->obj->_maxs.y);

            _mapMins.z = std::min(_mapMins.z, node.second->obj->_mins.z);
            _mapMaxs.z = std::max(_mapMaxs.z, node.second->obj->_maxs.z);
    }
    printf("Detected Map boundaries are:\n");
    float mapXSize = _mapMaxs.x - _mapMins.x;
    float mapYSize = _mapMaxs.y - _mapMins.y;
    float mapZSize = _mapMaxs.z - _mapMins.z;
    float floorHeight = minYNode->obj->_maxs.y;
    printf("\tX: %f <-> %f\t-> %f\n", _mapMins.x, _mapMaxs.x, mapXSize);
    printf("\tY: %f <-> %f\t-> %f\n", _mapMins.y, _mapMaxs.y, mapYSize);
    printf("\tZ: %f <-> %f\t-> %f\n", _mapMins.z, _mapMaxs.z, mapZSize);
    printf("Floor level detected at y= %f for node %s\n", floorHeight, minYNode->name.c_str());
    _nodeRes = std::max(mapXSize / _scanResolution, mapZSize / _scanResolution);
    printf("A* scan resolution size = %f units\n", _nodeRes);
    // Create scanning node and object for collision detection
    Node *collisionNode = new Node();
    collisionNode->name = "collisionNode";
    collisionNode->obj = new Obj("collisionNode");
    collisionNode->modelMatrix = glm::mat4(1.0f);
    collisionNode->obj->_mins = glm::vec3(-_nodeRes / 2, 0.0, -_nodeRes / 2);
    collisionNode->obj->_maxs = glm::vec3(_nodeRes / 2, 2.0, _nodeRes / 2);
    collisionNode->checkCollisions = false;
    _sceneTree.translateNode(collisionNode, glm::vec3(_mapMins.x + _nodeRes / 2, 0.2, _mapMins.z + _nodeRes / 2));
    printf("Starting at position %f, %f, %f\n",
                collisionNode->modelMatrix[3].x,
                collisionNode->modelMatrix[3].y,
                collisionNode->modelMatrix[3].z);
    collisionNode->checkCollisions = true;

    printf("%sStarting map collision analysis%s\n", C_GREEN, C_RESET);
    scanMap(collisionNode, _nodeRes, _mapMins);
    delete collisionNode->obj;
    delete collisionNode;
    return SUCCESS;
}

int App::scanMap(Node *collisionNode, float scanRes, glm::vec3 _mapMins)
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
    }
    for (int z = 0; z < _scanResolution; z++) {
        for (int x = 0; x < _scanResolution; x++) {
            if (_mapAnalysis[z][x])
                std::cout << '-';
            else
                std::cout << 'X';
        }
        std::cout << '\n';
    }
    return SUCCESS;
}
