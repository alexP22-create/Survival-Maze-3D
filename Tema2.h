#pragma once

#include "components/simple_scene.h"
#include "Tema2Camera.h"
#include "MazePlayer.h"
#include "transform3D.h"
#include "AttackProjectile.h"
#include "Maze.h"
#include "MazeEnemy.h"
#include "Hud.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void InitPlayer();
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void CreateCube(float length, char* mesh_name, glm::vec3 color);
        void CreateRectangular(float width, float length, float depth, char* mesh_name, glm::vec3 color);
        void DisplayPlayer();
        void DisplayProjectiles(float deltaTimeSeconds);
        void DisplayMaze();
        void DisplayEnemies(float deltaTime);
        void DisplayHud();
        void RenderDeformedMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color);

     protected:
        Tema2Camera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        bool won_game = false;

        MazePlayer* player;
        AttackProjectile* projectile;
        Maze* maze;
        MazeEnemy* enemy;
        Hud* hud;

        //initial positions 
        float distanceToTargetThirdPersonCamera = 0.9f;
        float distanceToTargetFirstPersonCamera;
        glm::vec3 thirdPersonCameraPosition = glm::vec3(0, 2.2, 0.9f);
        //scad 100 caci centrul la care se uita camera trb sa fie dupa ea
        glm::vec3 thirdPersonCenter = glm::vec3(0, 2.2, thirdPersonCameraPosition[2]-100);
        glm::vec3 firstPersonCameraPosition;
        glm::vec3 firstPersonCenter;
        bool isThirdPersonCamera = true;
        bool isInAttackMode = false;
        float dist_perspectives_xoz;
        float dist_perspectives_oy;

        //de la lab
        int perspOrtho;
        float FoV = RADIANS(60);
        float Left = -12;
        float Right = 12;
        float bottom = -6;
        float top = 6;
        float zNear = 0;
        float zFar = 100;
        //distanta punctului la care se uita camera
        float distanceToCenter = 10000;
        float distanceProjectile;

    };
}
