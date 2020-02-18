#include "aStar.hpp"


int aStar::scanNeighbors(int nodeIdx)
{
    if (!_map[nodeIdx]->_isFree)
        return 1;
    for (auto &delta : _deltas) {
        int tmpX = delta.first + nodeIdx % _mapWidth;
        int tmpY = delta.second + nodeIdx / _mapWidth;
        int neighborIdx = tmpY * _mapWidth + tmpX;
        if (tmpX >= 0 && tmpX < _mapWidth &&
            tmpY >= 0 && tmpY < _mapWidth &&
            _map[neighborIdx]->_isFree) {
            // printf("NavNode idx: %d, added neighbor %d\n", nodeIdx, neighborIdx);
            _map[nodeIdx]->links.push_back(_map[neighborIdx]);
        }
    }
    return 1;
}

int aStar::getNeighbor(int nodeIdx)
{
    for (auto &delta : _deltas) {
        int tmpX = delta.first + nodeIdx % _mapWidth;
        int tmpY = delta.second + nodeIdx / _mapWidth;
        int neighborIdx = tmpY * _mapWidth + tmpX;
        if (tmpX >= 0 && tmpX < _mapWidth &&
            tmpY >= 0 && tmpY < _mapWidth &&
            _map[neighborIdx]->_isFree) {
            return neighborIdx;
        }
    }
    return -1;
}

void aStar::displayMap()
{
    for (size_t i = 0; i < _map.size(); i++) {
        if (i % _mapWidth == 0 && i)
            std::cout << '\n';
        if (_map[i]->_isFree)
            std::cout << "-";
        else
            std::cout << "=";
    }
    std::cout << '\n';
}

void aStar::displayMap(std::vector<int> res)
{
    for (size_t i = 0; i < _map.size(); i++) {
        if (i % _mapWidth == 0 && i)
            std::cout << '\n';
        if (std::find(res.begin(), res.end(), i) != res.end())
            std::cout << "o";
        else if (_map[i]->_isFree)
            std::cout << "-";
        else
            std::cout << "=";
    }
    std::cout << '\n';
}

int aStar::initMap(std::vector<std::vector<bool>> collisionMap)
{
    _mapWidth = collisionMap.size();
    for (int x = 0; x < _mapWidth; x++) {
        for (int y = 0; y < _mapWidth; y++) {
            NavNode *tmpNode = new NavNode(collisionMap[x][y], toFlatArray(y, x));
            _map.push_back(tmpNode);
        }
    }
    for (size_t i = 0; i < _map.size(); i++)
        scanNeighbors(i);
    displayMap();
    return 1;
}

float aStar::getDistance(int idxA, int idxB)
{
    int xStart = idxA % _mapWidth;
    int yStart = idxA / _mapWidth;
    int xEnd = idxB % _mapWidth;
    int yEnd = idxB / _mapWidth;
    return ((float)sqrt((xStart - xEnd) * (xStart - xEnd) +
                            (yStart - yEnd) * (yStart - yEnd)));
}

void eraseFromVector(std::vector<NavNode *> &set, NavNode *toRemove)
{
	for (auto it = set.begin(); it != set.end(); it++) {
		if (*it == toRemove) {
            set.erase(it);
            return;
        }
    }
}

NavNode *aStar::getMinFScoreNode(std::vector<NavNode *> &tmpSet)
{
    float min = 999999999;
    NavNode *minNode = NULL;
    for (auto &tmpNode: tmpSet) {
        if (tmpNode->_fScore < min) {
            min = tmpNode->_fScore;
            minNode = tmpNode;
        }
    }
    return minNode;
}

std::vector<std::pair<int, int>> aStar::rebuildPath(
            std::map<int, int> cameFrom, int currentIdx, int startIdx)
{
	std::vector<int> tmp;
	tmp.push_back(currentIdx);

	while (currentIdx != startIdx) {
		currentIdx = cameFrom[currentIdx];
		tmp.push_back(currentIdx);
	}
	std::reverse(tmp.begin(), tmp.end());

    std::vector<std::pair<int, int>> res;
    for (auto &tmpIdx : tmp) {
        res.push_back(std::pair<int, int>(tmpIdx % _mapWidth, tmpIdx / _mapWidth));
	}
	return res;
}


std::vector<std::pair<int, int>> aStar::compute(int startIdx, int endIdx)
{
    std::vector<NavNode *> tmpSet = {_map[startIdx]};
    std::map<int, int> cameFrom;
    std::vector<std::pair<int, int>> res;
    resetCostFunctions();
    _map[startIdx]->_gScore = 0;
    _map[startIdx]->_fScore = getDistance(startIdx, endIdx);

    NavNode *currentPos;
    while (tmpSet.size()) {
        currentPos = getMinFScoreNode(tmpSet);
        if (currentPos->_idx == endIdx)
            return rebuildPath(cameFrom, currentPos->_idx, startIdx);
        eraseFromVector(tmpSet, currentPos);

        for (auto &neighbor : currentPos->links) {
            if (currentPos->_gScore + 1 < neighbor->_gScore) {
                cameFrom.insert(
                        std::pair<int, int>(neighbor->_idx, currentPos->_idx));
                neighbor->_gScore = currentPos->_gScore + 1;
                neighbor->_fScore = neighbor->_gScore +
                            getDistance(neighbor->_idx, endIdx);

                if (std::find(tmpSet.begin(), tmpSet.end(), neighbor) == tmpSet.end())
					tmpSet.push_back(neighbor);
            }
        }
    }
    return res;
}
