#include "MazePlayer.h"
#include <vector>
#include <iostream>

void MazePlayer::moveForward(float deltaTime) {
    float distance = 2 * deltaTime;
    float ox_projection = -sin(this->angle) * distance;
    float oz_projection = -cos(this->angle) * distance;
    this->position[0] += ox_projection;
    this->position[2] += oz_projection;
}
void MazePlayer::moveLeft(float deltaTime) {
    float distance = 2 * deltaTime;
    float ox_projection = cos(this->angle) * distance;
    float oz_projection = sin(this->angle) * distance;
    this->position[0] -= ox_projection;
    this->position[2] += oz_projection;
}
void MazePlayer::moveRight(float deltaTime) {
    float distance = 2 * deltaTime;
    float ox_projection = cos(this->angle) * distance;
    float oz_projection = sin(this->angle) * distance;
    this->position[0] += ox_projection;
    this->position[2] -= oz_projection;
}
void MazePlayer::moveBackward(float deltaTime) {
    float distance = 2 * deltaTime;
    float ox_projection = sin(this->angle) * distance;
    float oz_projection = cos(this->angle) * distance;
    this->position[0] += ox_projection;
    this->position[2] += oz_projection;
}
void MazePlayer::setStartPosition(Maze* maze) {
    //from 2D in 3D
    this->position[0] = maze->length * maze->start_x + maze->length / 2;
    this->position[2] = maze->length * maze->start_y + maze->length / 2;
}
bool MazePlayer::checkWallCollision(Maze* maze) {
    //find the coordinates for the player
    float player_minX = this->position[0] - this->length_rectangle;
    float player_minY = 0;
    float player_minZ = this->position[2] - this->length_rectangle;
    float player_maxX = this->position[0] + this->length_rectangle;
    float player_maxY = this->position[1] + 2* this->length_rectangle + this->length_cube;
    float player_maxZ = this->position[2] + this->length_rectangle;

    //the coordinates of a maze block
    float cube_minX;
    float cube_minY;
    float cube_minZ;
    float cube_maxX;
    float cube_maxY;
    float cube_maxZ;


    for (int i = 0; i < maze->n; i++) {
        for (int j = 0; j < maze->n; j++) {
            if (maze->map[i][j] == 1) {
                float x = maze->length / 2 + maze->length * i;
                float z = maze->length / 2 + maze->length * j;
                float y = maze->length;

                cube_minX = x - maze->length / 2;
                cube_maxX = x + maze->length / 2;
                cube_minZ = z - maze->length / 2;
                cube_maxZ = z + maze->length / 2;
                cube_minY = 0;
                cube_maxY = y;

                //check if the player collides with the current maze block
                if ((player_minX <= cube_maxX && player_maxX >= cube_minX) &&
                    (player_minY <= cube_maxY && player_maxY >= cube_minY) &&
                    (player_minZ <= cube_maxZ && player_maxZ >= cube_minZ)) {
                    return true;
                }
            }
        }
    }
    return false;
    
}

bool MazePlayer::checkEnemyCollision(MazeEnemy* enemy, Maze* maze) {
    //find the coordinates for the player
    float player_minX = this->position[0] - this->length_rectangle;
    float player_minY = 0;
    float player_minZ = this->position[2] - this->length_rectangle;
    float player_maxX = this->position[0] + this->length_rectangle;
    float player_maxY = this->position[1] + 2 * this->length_rectangle + this->length_cube;
    float player_maxZ = this->position[2] + this->length_rectangle;

    //the coordinates of an enemy block
    float enemy_minX;
    float enemy_minY;
    float enemy_minZ;
    float enemy_maxX;
    float enemy_maxY;
    float enemy_maxZ;


    for (int i = 0; i < maze->n; i++) {
        for (int j = 0; j < maze->n; j++) {
            if (maze->map[i][j] == 2) {
                double x = (double)enemy->position[0] + maze->length / 2 + maze->length * i;
                double z = (double)enemy->position[2] + maze->length / 2 + maze->length * j;
                float y = enemy->height;

                enemy_minX = x - enemy->length / 2;
                enemy_maxX = x + enemy->length / 2;
                enemy_minZ = z - enemy->length / 2;
                enemy_maxZ = z + enemy->length / 2;
                enemy_minY = 0;
                enemy_maxY = y;

                //check if the player collides with the current enemy block
                if ((player_minX <= enemy_maxX && player_maxX >= enemy_minX) &&
                    (player_minY <= enemy_maxY && player_maxY >= enemy_minY) &&
                    (player_minZ <= enemy_maxZ && player_maxZ >= enemy_minZ)) {
                    maze->map[i][j] = 0;
                    return true;
                }
            }
        }
    }
    return false;

}

//a ajuns la final cand a ajuns la bucata de drum de la iesire
//coliziune cu cub-ul care ar fi fost pe bucata
bool MazePlayer::checkReachedEnd(Maze* maze) {
    //find the coordinates for the player
    float player_minX = this->position[0] - this->length_rectangle;
    float player_minY = 0;
    float player_minZ = this->position[2] - this->length_rectangle;
    float player_maxX = this->position[0] + this->length_rectangle;
    float player_maxY = this->position[1] + 2 * this->length_rectangle + this->length_cube;
    float player_maxZ = this->position[2] + this->length_rectangle;

    //the coordinates of the imaginary maze block
    float cube_minX;
    float cube_minY;
    float cube_minZ;
    float cube_maxX;
    float cube_maxY;
    float cube_maxZ;

    float x = maze->length / 2 + maze->length * maze->end_x;
    float z = maze->length / 2 + maze->length * maze->end_y;
    float y = maze->length;

    cube_minX = x - maze->length / 2;
    cube_maxX = x + maze->length / 2;
    cube_minZ = z - maze->length / 2;
    cube_maxZ = z + maze->length / 2;
    cube_minY = 0;
    cube_maxY = y;

    //check if the player collides with the current maze block
    if ((player_minX <= cube_maxX && player_maxX >= cube_minX) &&
        (player_minY <= cube_maxY && player_maxY >= cube_minY) &&
        (player_minZ <= cube_maxZ && player_maxZ >= cube_minZ)) {
        return true;
    }
    return false;
}