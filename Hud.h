#pragma once

#include <string>
#include "core/gpu/mesh.h"
#include <vector>
//the hud contains 4 rectangles(2 for the healthbar of the player and 2 for the remaining time bar)
class Hud {
public:
	Mesh* CreateBar(
		const std::string& name,
		glm::vec3 leftBottomCorner,
		float lengthX,
		float lengthY,
		glm::vec3 color);
	Mesh* CreateFullBar(
		const std::string& name,
		glm::vec3 leftBottomCorner,
		float lengthX,
		float lengthY,
		glm::vec3 color);
	float damageTaken = 0;
	float remainingTime = 1;
	//lungimile urm sunt in pixeli pt viewport
	const float lengthX = 250;
	const float lengthY = 25; 
	glm::vec3 position;
};