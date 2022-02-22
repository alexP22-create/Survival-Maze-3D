#include <vector>
#include <iostream>
#include "AttackProjectile.h"

void AttackProjectile::eliminate(int i) {
    this->times.erase(this->times.begin() + i);
    this->angles.erase(this->angles.begin() + i);
    this->positions.erase(this->positions.begin() + i);
}

bool AttackProjectile::checkCollisionMaze(int i, Maze* maze) {

    glm::vec3 position = this->positions[i];

    //find the coordinates for the sphere
    float sphere_minX = position[0] - this->radius;
    float sphere_minY = position[1] - this->radius;
    float sphere_minZ = position[2] - this->radius;
    float sphere_maxX = position[0] + this->radius;
    float sphere_maxY = position[1] + this->radius;
    float sphere_maxZ = position[2] + this->radius;

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
                //the values of the center of the cube object
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
                if ((sphere_minX <= cube_maxX && sphere_maxX >= cube_minX) &&
                    (sphere_minY <= cube_maxY && sphere_maxY >= cube_minY) &&
                    (sphere_minZ <= cube_maxZ && sphere_maxZ >= cube_minZ)) {
                    //std::cout << maze->start_x << " " << maze->start_y << "\n";
                    //std::cout << i << " " << j << "\n";
                    return true;
                }
            }
        }
    }
    return false;

}

bool AttackProjectile::checkCollisionEnemy(int i, MazeEnemy* enemy, Maze* maze) {
    glm::vec3 position = this->positions[i];

    //find the coordinates for the sphere
    float sphere_minX = position[0] - this->radius;
    float sphere_minY = position[1] - this->radius;
    float sphere_minZ = position[2] - this->radius;
    float sphere_maxX = position[0] + this->radius;
    float sphere_maxY = position[1] + this->radius;
    float sphere_maxZ = position[2] + this->radius;

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

                //the values of the center of the enemy object

                enemy_minX = x - enemy->length / 2;
                enemy_maxX = x + enemy->length / 2;
                enemy_minZ = z - enemy->length / 2;
                enemy_maxZ = z + enemy->length / 2;
                enemy_minY = 0;
                enemy_maxY = y;

                //check if the sphere collides with the current enemy block
                if ((sphere_minX <= enemy_maxX && sphere_maxX >= enemy_minX) &&
                    (sphere_minY <= enemy_maxY && sphere_maxY >= enemy_minY) &&
                    (sphere_minZ <= enemy_maxZ && sphere_maxZ >= enemy_minZ)) {
                    enemy->destroyed_time.push_back(3);
                    enemy->destroyed_x.push_back(x);
                    enemy->destroyed_z.push_back(z);
                    maze->map[i][j] = 0;
                    return true;
                }
            }
        }
    }
    return false;

}