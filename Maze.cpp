#include "Maze.h"
#include <vector>
#include <iostream>

//finds the exit from the final found passable by player position
void Maze::compute_exit() {

	bool valid_exit = false;
	while (valid_exit == false) {
		//pic a random position connected to the outer walls
		end_x = 1 + (std::rand() % (n - 2 - 1 + 1));
		if (end_x == 1 || end_x == n - 2) {
			end_y = 1 + (std::rand() % (n - 2 - 1 + 1));
		}
		else {
			int end_y = std::rand() % 2;
			if (end_y == 0)
				end_y = 1;
			else
				end_y = n - 2;
		}

		//if that found position is a wall look for another
		if (map[end_x][end_y] == 1)
			continue;

		//check if it's near an outer wall and make the exit in the wall
		if (end_x - 1 == 0) {
			end_x = 0;
			valid_exit = true;
		}
		else
			if (end_x + 1 == n - 1) {
				end_x = n - 1;
				valid_exit = true;
			}
			else
				if (end_y - 1 == 0) {
					end_y = 0;
					valid_exit = true;
				}
				else
					if (end_y + 1 == n - 1) {
						end_y = n - 1;
						valid_exit = true;
					}
		}
	map[end_x][end_y] = 0;
}

void Maze::print_map() {
	std::cout << "======================================================\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << map[i][j];
			std::cout << " ";
		}
		std::cout << "\n";
	}
}

bool Maze::available_position(int x, int y) {
	//not already set
	if (map[x][y] != -1)
		return false;

	//sa nu se lipeasca 2 drumuri
	int nr_0 = 0;
	if (map[x+1][y] == 0 || map[x+1][y] == 2)
		nr_0 ++;
	if (map[x - 1][y] == 0 || map[x - 1][y] == 2)
		nr_0++;
	if (map[x][y + 1] == 0 || map[x][y + 1] == 2)
		nr_0++;
	if (map[x][y - 1] == 0 || map[x][y - 1] == 2)
		nr_0++;

	//nu pot fi mai mult de 2 pozitii de mers una langa alta
	if (nr_0 >= 2)
		return false;
	return true;
}

void Maze::build_maze(int x, int y) {
	//mark the map only if it's not setted
	if (map[x][y] == -1) {
		map[x][y] = 0;

		//15% chance to be an enemy
		if (std::rand() % 7 == 0) {
			map[x][y] = 2;
		}
		//ad the found position to the queue
		std::vector<int> vec;
		vec.push_back(x);
		vec.push_back(y);
		positions.push(vec);
	}
	//list of available paths
	std::vector<int> available_paths;
	available_paths.push_back(0);
	available_paths.push_back(1);
	available_paths.push_back(2);
	available_paths.push_back(3);
	
	bool found = false;
	//check all the paths untill found a valid one or none
	while (found == false) {
		if (available_paths.size() == 0) {
			found = false;
			break;
		}

		//next position
		int position = std::rand() % available_paths.size();
		int next = available_paths.at(position);

		switch (next)
		{
		case 0:
			//jos
			if (available_position(x, y - 1)) {
				y -= 1;
				found = true;
			}
			else {
				available_paths.erase(available_paths.begin() + position);
			}
			break;
		case 1:
			//stanga
			if (available_position(x - 1, y)) {
				x -= 1;
				found = true;
			}
			else {
				available_paths.erase(available_paths.begin() + position);
			}
			break;
		case 2:
			//sus
			if (available_position(x, y + 1)) {
				y += 1;
				found = true;
			}
			else {
				available_paths.erase(available_paths.begin() + position);
			}
			break;
		case 3:
			//dreapta
			if (available_position(x + 1, y)) {
				x += 1;
				found = true;
			}
			else {
				available_paths.erase(available_paths.begin() + position);
			}
			break;
		default:
			break;
		}
	}

	if (found == true)
		build_maze(x, y);
}

//generate the labyrinth with the backtracking algorithm
void Maze::generate_maze_map() {
	//initialize it with -1
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			map[i][j] = -1;
		}
	}
	//set the labyrint outer walls
	for (int i = 0; i < n; i++) {
		map[i][0] = 1;
		map[n - 1][i] = 1;
		map[0][i] = 1;
		map[i][n - 1] = 1;
	}
	//randomly chose the starting cell between 1 and n - 2 because 0 and n - 1 are walls
	int x = 1 + (std::rand() % (n - 2 - 1 + 1));
	int y = 1 + (std::rand() % (n - 2 - 1 + 1));
	start_x = x;
	start_y = y;
	map[start_x][start_y] = 0;


	build_maze(x, y);
	//continue to check if there are more positions left
	
	while (positions.size() != 0) {
		//this->print_map();
		std::vector<int> last_added = positions.top();
		positions.pop();
		int last_x = last_added.at(0);
		int last_y = last_added.at(1);
		build_maze(last_x, last_y);
	}

	//finnish the walls
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (map[i][j] == -1) {
				map[i][j] = 1;
			}
		}
	}

	this->compute_exit();
	this->print_map();
}
