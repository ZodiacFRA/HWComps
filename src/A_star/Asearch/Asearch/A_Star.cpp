#include <iostream>
#include <list>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
using namespace std;

struct pos {
	int x;
	int y;

	pos(int xPos, int yPos)
		: x(xPos), y(yPos)
	{}

	pos()
	{
		x = -1;
		y = -1;
	}

	bool operator==(pos that)
	{
		return (this->x == that.x && this->y == that.y);
	}

	bool operator!=(pos that)
	{
		return (this->x != that.x || this->y != that.y);
	}
};


void print(std::vector<pos> input)
{
	for (auto pos : input)
		std::cout << pos.x << "/" << pos.y << std::endl;
}


void set_values_to_max(std::vector<std::vector<bool>> map) {
	int i_max = map.size();
	int j_max = map[0].size();

	for (int i = 0; i < i_max; i++) {
		for (int j = 0; j < j_max; j++) {
			map.at(i).at(j) = 999999.0;
		}
	}
}

std::vector<pos> get_neightbours(std::vector<std::vector<bool>> map, pos centre) {

	std::vector<pos> neightbours_array;
	int max_lenght = map.size();


	if (centre.x > 0 && centre.y > 0) {
		if (map[centre.x - 1][centre.y - 1]) {

			if(map[centre.x][centre.y - 1] || map[centre.x - 1][centre.y]) {
				pos vector_neighbour_1 = pos(centre.x - 1, centre.y - 1);

				neightbours_array.push_back(vector_neighbour_1);
			}
			
		}
	}
	if (centre.x > 0) {
		if (map[centre.x - 1][centre.y]) {

			pos vector_neighbour_2 = pos(centre.x - 1, centre.y);

			neightbours_array.push_back(vector_neighbour_2);
		}
	}
	if (centre.x > 1 && centre.y < max_lenght - 1) {
		if (map[centre.x - 1][centre.y + 1]) {

			if (map[centre.x-1 ][centre.y] || map[centre.x][centre.y+1]) {
				pos vector_neighbour_3 = pos(centre.x - 1, centre.y + 1);

				neightbours_array.push_back(vector_neighbour_3);
			}
		}
	}
	if (centre.y > 0) {
		if (map[centre.x][centre.y - 1]) {
			pos vector_neighbour_4 = pos(centre.x, centre.y - 1);

			neightbours_array.push_back(vector_neighbour_4);
		}
	}
	if (centre.y < max_lenght - 1) {
		if (map[centre.x][centre.y + 1]) {
			pos vector_neighbour_5 = pos(centre.x, centre.y + 1);

			neightbours_array.push_back(vector_neighbour_5);
		}
	}
	if (centre.x < max_lenght - 1 && centre.y > 0) {
		if (map[centre.x + 1][centre.y - 1]) {
			if (map[centre.x][centre.y-1] || map[centre.x+1][centre.y]) {
				pos vector_neighbour_6 = pos(centre.x + 1, centre.y - 1);

				neightbours_array.push_back(vector_neighbour_6);
			}
		}
	}
	if (centre.x < max_lenght - 1) {
		if (map[centre.x + 1][centre.y]) {
			pos vector_neighbour_7 = pos(centre.x + 1, centre.y);

			neightbours_array.push_back(vector_neighbour_7);
		}
	}
	if (centre.x < max_lenght - 1 && centre.y < max_lenght - 1) {
		if (map[centre.x + 1][centre.y + 1]) {
			if (map[centre.x+1][centre.y] || map[centre.x][centre.y + 1]) {
				pos vector_neighbour_8 = pos(centre.x + 1, centre.y + 1);

				neightbours_array.push_back(vector_neighbour_8);
			}
		}
	}

	return neightbours_array;

}


std::pair<pos, float> getFloatFromPos(std::vector<std::pair<pos, float>> fscore_map, pos wanted_pos)
{
	for (auto tmpPair : fscore_map) {
		if (wanted_pos == tmpPair.first)
			return tmpPair;
	}
	return std::pair<pos, float>(pos(), 9999999999);
}


pos get_min_heuristic(std::vector<pos> open_set, std::vector<std::pair<pos, float>> fscore_map)
{
	float min_h = 9999999999;
	pos min_pos = pos(-1, -1);

	for (auto tmpPos : open_set) {
		std::pair<pos, float> casePair = getFloatFromPos(fscore_map, tmpPos);
		if (casePair.second < min_h) {
			min_h = casePair.second;
			min_pos = casePair.first;
		}
	}
	return min_pos;
}

float calculate_heuristic(pos A, pos B)
{
	return ((float)sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y)));
}

int getPositionIndex(std::vector<std::pair<pos, float>> gscore_map, pos target)
{
	for (int i = 0; i < gscore_map.size(); i++) {
		if (gscore_map[i].first == target)
			return i;
	}
	return -1;
}

int getPositionIndex(std::vector<std::pair<pos, pos>> gscore_map, pos target)
{
	for (int i = 0; i < gscore_map.size(); i++) {
		if (gscore_map[i].first == target)
			return i;
	}
	return -1;
}

std::vector<pos> reconstruct_path(std::vector<std::pair<pos, pos>> cameFrom_map, pos current_location, pos start_location)
{

	std::vector<pos> path;
	path.push_back(current_location);

	while (current_location != start_location) {
		current_location = cameFrom_map[getPositionIndex(cameFrom_map, current_location)].second;
		path.push_back(current_location);
	}

	std::reverse(path.begin(), path.end());
	return path;
}

std::vector<pos> erase_from_vector(std::vector<pos> set, pos vector) {

	for (int i = 0; i < set.size(); i++) {
		if (set[i] == vector) {
			set.erase(set.begin() + i);
			return set;
		}
	}
	return set;
}

bool isInVector(pos target, std::vector<pos> set)
{
	for (auto tmpPos : set) {
		if (target == tmpPos) {
			return true;
		}
	}
	return false;
}

std::vector<pos> a_star(std::vector<std::vector<bool>> game_map, pos start_location, pos end_location)
{
	std::vector<pos> open_set = { start_location };
	std::vector<std::pair<pos, pos>> cameFrom_map;
	std::vector<pos> path;
	pos current_location;

	// Init score maps
	std::vector<std::pair<pos, float>> gscore_map;
	for (int x = 0; x < game_map.size(); x++) {
		for (int y = 0; y < game_map.size(); y++) {
			gscore_map.push_back(std::pair<pos, float>(pos(x, y), 999999999));
		}
	}
	gscore_map[getPositionIndex(gscore_map, start_location)].second = 0;

	std::vector<std::pair<pos, float>> fscore_map;
	for (int x = 0; x < game_map.size(); x++) {
		for (int y = 0; y < game_map.size(); y++) {
			fscore_map.push_back(std::pair<pos, float>(pos(x, y), 999999999));
		}
	}
	fscore_map[getPositionIndex(fscore_map, start_location)].second = calculate_heuristic(start_location, end_location); // estimate the heuristic of the start location

	// Start algo
	while (open_set.size() != 0) {
		// std::cout << "---------------" << '\n';
		// std::cout << "size: " << open_set.size() << '\n';
		current_location = get_min_heuristic(open_set, fscore_map);

		if (current_location == end_location)
			return reconstruct_path(cameFrom_map, current_location, start_location);

		open_set = erase_from_vector(open_set, current_location);
		// std::cout << "size: " << open_set.size() << '\n';

		std::vector<pos> neightbours = get_neightbours(game_map, current_location);
		// std::cout << "neigh size " << neightbours.size() << '\n';
		for (auto current_neightbour : neightbours) {

			float tentative_gscore = gscore_map[getPositionIndex(gscore_map, current_location)].second + 1;
			// std::cout << "tentative_gscore: " << tentative_gscore << '\n';
			if (tentative_gscore < gscore_map[getPositionIndex(gscore_map, current_neightbour)].second) {
				// This path to neighbor is better than any previous one. Record it!
				// cameFrom_map[current_neightbour] = current_location;
				cameFrom_map.push_back(std::pair<pos, pos>(current_neightbour, current_location));
				gscore_map[getPositionIndex(gscore_map, current_neightbour)].second = tentative_gscore;
				fscore_map[getPositionIndex(fscore_map, current_neightbour)].second = gscore_map[getPositionIndex(gscore_map, current_neightbour)].second + calculate_heuristic(current_neightbour, end_location);

				if (!isInVector(current_neightbour, open_set)) {
					open_set.push_back(current_neightbour);
				}
			}
		}
	}
	return path;
}


std::vector<std::vector<bool>> import_text_file(string file_name) 
{

	std::vector<std::vector<bool>> map;
	std::ifstream file(file_name);
	std::string str;
	int i = 0;
	while (std::getline(file, str)) {
		// std::cout << str << "\n";

		std::vector<bool> line;

		for (int j = 0; j < str.length(); j++) {
			if (str[j]== 1) {
				line.push_back(false);
			}
			if (str[j] == 0) 
			{
				line.push_back(false);
			}
		}
		map.push_back(line);
		i++;
	}

	return map;
}


int main()
{
	// start();
	/**
	std::vector<std::vector<bool>> game_map = {
		{true, true, false, false, false, false},
		{true, false, true, false, false, false},
		{true, true, true, true, false, false},
		{true, true, true, true, true, false},
		{true, true, true, true, true, true},
		{true, true, true, true, true, true} };
**/
		
	/*
	std::vector<std::vector<bool>> game_map = {
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false},
		{true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false},
		{true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false},
		{true, false, true, true, true, true, true, true, false, false, true, true, true, false, true, true, true, true, true, true, false, false, true, true},
		{true, false, true, true, false, false, true, true, false, false, true, true, true, false, true, true, false, false, true, true, false, false, true, true},
		{true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false},
		{true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false},
		{false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
		{true, false, true, true, true, true, true, true, false, false, true, true, true, false, true, true, true, true, true, true, false, false, true, true},
		{true, false, true, true, false, false, true, true, false, false, true, true, true, false, true, true, false, false, true, true, false, false, true, true},
		{true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true}
		};
	 */	

	std::vector<std::vector<bool>> game_map = {
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true,
			false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true
			, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true,
			false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false,
			true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false},
		{true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false,
			true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false},
		{true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false,
			true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false},
		{true, true, false, true, true, true, true, true, true, false, false, true, true, true, false, true, true, true, true, true, true, false, false, true, true,
			true, true, false, true, true, true, true, true, true, false, false, true, true, true, false, true, true, true, true, true, true, false, false, true, true},
		{true, true, false, true, true, false, false, true, true, false, false, true, true, true, false, true, true, false, false, true, true, false, false, true, true,
			true, true, false, true, true, false, false, true, true, false, false, true, true, true, false, true, true, false, false, true, true, false, false, true, true},
		{true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true,
			true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true,
			false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true,
			false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true,
			false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, true, true,
			true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, true, true},
		{true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, true, true,
			true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, true, true},
		{true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
		{true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
		{true, true, false, true, true, true, true, true, true, false, false, true, true, true, false, true, true, true, true, true, true, false, false, true, true,
			true, true, false, true, true, true, true, true, true, false, false, true, true, true, false, true, true, true, true, true, true, false, false, true, true},
		{true, true, false, true, true, false, false, true, true, false, false, true, true, true, false, true, true, false, false, true, true, false, false, true, true,
			true, true, false, true, true, false, false, true, true, false, false, true, true, true, false, true, true, false, false, true, true, false, false, true, true},
		{true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true,
			true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true,
			false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true
			, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true,
			false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false,
			true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false},
		{true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false,
			true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false},
		{true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false,
			true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false},
		{true, true, false, true, true, true, true, true, true, false, false, true, true, true, false, true, true, true, true, true, true, false, false, true, true,
			true, true, false, true, true, true, true, true, true, false, false, true, true, true, false, true, true, true, true, true, true, false, false, true, true},
		{true, true, false, true, true, false, false, true, true, false, false, true, true, true, false, true, true, false, false, true, true, false, false, true, true,
			true, true, false, true, true, false, false, true, true, false, false, true, true, true, false, true, true, false, false, true, true, false, false, true, true},
		{true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true,
			true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true,
			false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true,
			false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true,
			false,false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
			true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true},
		{true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, true, true,
			true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, true, true},
		{true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, true, true,
			true, true, true, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, true, true},
		{true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
		{true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
		{true, true, false, true, true, true, true, true, true, false, false, true, true, true, false, true, true, true, true, true, true, false, false, true, true,
			true, true, false, true, true, true, true, true, true, false, false, true, true, true, false, true, true, true, true, true, true, false, false, true, true},
		{true, true, false, true, true, false, false, true, true, false, false, true, true, true, false, true, true, false, false, true, true, false, false, true, true,
			true, true, false, true, true, false, false, true, true, false, false, true, true, true, false, true, true, false, false, true, true, false, false, true, true},
		{true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true,
			true, true, true, true, true, false, false, true, true, true, true, true, true, true, true, true, true, false, false, true, true, true, true, true, true}
	};


	// std::vector<std::vector<bool>> game_map = import_text_file("input.txt");

	pos start_location = pos(0, 0);
	
	pos end_location = pos(49,49);

	std::vector<pos> path = a_star(game_map, start_location, end_location);
	std::cout << "=====================" << '\n';
	for (int i = 0; i < game_map.size(); i++) {
		for (int j = 0; j < game_map.size(); j++) {
			if (game_map[j][i])
				std::cout << "-";
			else
				std::cout << "#";
		}
		std::cout << '\n';
	}
	std::cout << "=====================" << '\n';
	for (int i = 0; i < game_map.size(); i++) {
		for (int j = 0; j < game_map.size(); j++) {
			if (isInVector(pos(j, i), path))
				std::cout << "O";
			else if (game_map[j][i])
				std::cout << "-";
			else
				std::cout << "#";
		}
		std::cout << '\n';
	}
	print(path);
	return 0;

}
