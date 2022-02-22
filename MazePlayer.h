#pragma once

#include <string>
#include "Maze.h"
#include "MazeEnemy.h"
#include "core/gpu/mesh.h"

class MazePlayer{
public:
	glm::vec3 position;
	//latimea unui cub din cele din care e construit player-ul
	float length_cube = 0.3;
	float length_rectangle = 0.9;
	float width_rectangle = 0.7;
	//coordonatele la care se gasesc ochii player-ului
	float eyes_z = -length_cube;
	float eyes_y = 2*length_rectangle;
	//unghiul la care se roteste omul
	float angle = 0;
	//coordonatele pozitiei initiale ale fiecarui obiect ce alcatuieste omul
	glm::vec3 left_fist = glm::vec3(-(length_cube/2 + width_rectangle/2), length_rectangle + length_cube/2, 0);
	glm::vec3 right_fist = glm::vec3(+(length_cube / 2 + width_rectangle / 2), length_rectangle + length_cube/2, 0);
	glm::vec3 body = glm::vec3(0, length_rectangle/2 + length_rectangle, 0);
	glm::vec3 head = glm::vec3(0, length_cube / 2 + length_rectangle * 2, 0);
	glm::vec3 left_arm = glm::vec3(-(length_cube / 2 + width_rectangle / 2), length_rectangle / 2 + length_rectangle + length_cube/2, 0);
	glm::vec3 right_arm = glm::vec3(+(length_cube / 2 + width_rectangle / 2), length_rectangle / 2 + length_rectangle +length_cube/2, 0);
	glm::vec3 left_leg = glm::vec3(-width_rectangle/2, length_rectangle / 2, 0);
	glm::vec3 right_leg = glm::vec3(+width_rectangle / 2, length_rectangle / 2, 0);

	void moveForward(float deltatime);
	void moveLeft(float deltatime);
	void moveRight(float deltatime);
	void moveBackward(float deltatime);
	void setStartPosition(Maze *maze);
	bool checkWallCollision(Maze *maze);
	bool checkEnemyCollision(MazeEnemy* enemy, Maze* maze);
	bool checkReachedEnd(Maze* maze);
};