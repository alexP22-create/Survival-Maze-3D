#pragma once

#include <string>
#include <stack>
#include <vector>
class Maze{
public:
	int map[10][10];
	int player_x;
	int player_y;
	const int n = 10;
	int start_x;
	int start_y;
	int end_x;
	int end_y;
	int nr = 0;
	double length;
	double height;
	//queue to store the positions in the map available to the player to move on 
	//used at the building of the maze
	std::stack<std::vector<int>> positions;

	void generate_maze_map();
	void build_maze(int x, int y);
	bool available_position(int x, int y);
	void print_map();
	void compute_exit();
};