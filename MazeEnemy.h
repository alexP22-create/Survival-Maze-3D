#pragma once

#include <string>
#include "core/gpu/mesh.h"
#include "Maze.h"

class MazeEnemy{
public:
	//for destroyed enemies
	std::vector <float> destroyed_x;
	std::vector <float> destroyed_z;
	std::vector <float> destroyed_time;
	//pozitia in care un inamic se misca raportata la o singura bucata din labirint
	glm::vec3 position = glm::vec3(0,0,0);
	//latimea unui cub din cele din care e construit inamicul
	const float length = 0.3;
	const float height = 3;
	//the limits in which an enemy can move
	float limit_x;
	float limit_z;
	void move(float deltaTime);
	void set_limits(float limit_x, float limit_z);
	//to knwo which checkpoints does the enemy want to reach in his movement
	//it firstly goes to right upper corner
	bool up_right = true;
	bool up_left = false;
	bool down_right = false;
	bool down_left = false;
	//the angle at which the enemy is moving, changeable
	float angle = 3.14/4;
	void eliminate(int i);
};