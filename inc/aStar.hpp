#include "common_HWComps.hpp"


class NavNode {
public:
    NavNode(bool isFree, int idx)
        : _fScore(999999999), _gScore(999999999), _isFree(isFree), _idx(idx)
    {}
    float _fScore;
    float _gScore;
    bool _isFree;
    std::vector<NavNode *> links;
    int _idx;
};

struct aStar {
    ~aStar() { for (size_t i = 0; i < _map.size(); i++) delete _map[i]; };
    void displayMap();
    int getNeighbor(int nodeIdx);
    void displayMap(std::vector<int> res);
    int initMap(std::vector<std::vector<bool>> map);
    int toFlatArray(int x, int y) { return y * _mapWidth + x; };
    std::vector<std::pair<int, int>> compute(int startIdx, int endIdx);
    std::vector<std::pair<int, int>> rebuildPath(
                std::map<int, int> pathHistory, int currentIdx, int startIdx);
    float getDistance(int idxA, int idxB);
    NavNode *getMinFScoreNode(std::vector<NavNode *> &tmpSet);
    void resetCostFunctions() {
        for (auto NavNode : _map) {
            NavNode->_fScore = 999999999;
            NavNode->_gScore = 999999999;
        }
    };
    int scanNeighbors(int nodeIndex);

    int _mapWidth;
    std::vector<NavNode *> _map;
    std::vector<std::pair<int, int>> _deltas = {
        std::make_pair(-1, 0),
        std::make_pair(-1, 1),
        std::make_pair(0, 1),
        std::make_pair(1, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(0, -1),
        std::make_pair(-1, -1),
    };
};
