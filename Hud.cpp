#include <vector>
#include <iostream>
#include "Hud.h"

Mesh* Hud::CreateBar(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float lengthX,
    float lengthY,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;


    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(lengthX, 0, 0), color),
        VertexFormat(corner + glm::vec3(lengthX, lengthY, 0), color),
        VertexFormat(corner + glm::vec3(0, lengthY, 0), color)
    };

    Mesh* square = new Mesh(name);

    std::vector<unsigned int> indices = { 0, 1, 2, 3, 0 };
    square->SetDrawMode(GL_LINE_LOOP);
    square->InitFromData(vertices, indices);
    return square;
}

Mesh* Hud::CreateFullBar(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float lengthX,
    float lengthY,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(lengthX, 0, 0), color),
        VertexFormat(corner + glm::vec3(lengthX, lengthY, 0), color),
        VertexFormat(corner + glm::vec3(0, lengthY, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };
    square->SetDrawMode(GL_TRIANGLES);
    square->InitFromData(vertices, indices);
    return square;
}