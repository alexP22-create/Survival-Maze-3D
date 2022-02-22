#include "MazeEnemy.h"
#include <vector>
#include <iostream>

void MazeEnemy::move(float deltaTime) {
	float distance = 3*deltaTime;
	float ox ;
	float oz;
	//go to upper right corner
	if (position[0] <= limit_x && position[2] <= limit_z && up_right) {
		ox = cos(angle) * distance;
		oz = sin(angle) * distance;

		position[0] += ox;
		position[2] += oz;
		return;
	} else
		if (up_right) {
			angle = 3.14/2;
			up_right = false;
			down_right = true;
		}

	//go to down right corner
	if (position[2] >= -limit_z && down_right) {
		oz = sin(angle) * distance;

		position[2] -= oz;
		return;
	}
	else
		if (down_right) {
			angle = 3.14 / 4;
			down_right = false;
			up_left = true;
		}

	//go to upper left corner
	if (position[0] >= -limit_x && position[2] <= limit_z && up_left) {
		ox = cos(angle) * distance;
		oz = sin(angle) * distance;

		position[0] -= ox;
		position[2] += oz;
		return;
	}
	else 
		if (up_left) {
			angle = 3.14/2;
			up_left = false;
			down_left = true;
		}

	//go to down left corner
	if (position[2] >= -limit_z && down_left) {
		oz = sin(angle) * distance;

		position[2] -= oz;
		return;
	}
	else if (down_left) {
			angle = 3.14 / 4;
			down_left = false;
			up_right = true;
		}

}

void MazeEnemy::set_limits(float x, float z) {
	limit_x = 2.0 / 3 * x;
	limit_z = 2.0 / 3 * z;
}

void MazeEnemy::eliminate(int i) {
	this->destroyed_time.erase(this->destroyed_time.begin() + i);
	this->destroyed_x.erase(this->destroyed_x.begin() + i);
	this->destroyed_z.erase(this->destroyed_z.begin() + i);
}