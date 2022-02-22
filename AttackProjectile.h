#pragma once

#include <string>
#include "core/gpu/mesh.h"
#include <vector>
#include "Maze.h"
#include "MazeEnemy.h"

class AttackProjectile{
public:
	float life_span = 6;
	//the times of life for each projectile
	std::vector <float> times;
	//the angle for each projectile
	std::vector <float> angles;
	//the coordonates of the fire position
	std::vector <glm::vec3> positions;

	float radius = 0.45;

	void eliminate(int i);

	bool checkCollisionMaze(int i, Maze* maze);

	bool checkCollisionEnemy(int i, MazeEnemy* enemy, Maze* maze);

};