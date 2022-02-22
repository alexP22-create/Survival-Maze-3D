#include "lab_m1/Tema2/Tema2.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  POSTOLACHE ALEXANDRU GABRIEL 331CB
 */

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

void Tema2::CreateRectangular(float total_width, float total_length, float total_depth, char* mesh_name, glm::vec3 color) {
    float length = total_length / 2;
    float width = total_width / 2;
    float depth = total_depth / 2;
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-width, -length,   depth), glm::vec3(0, length,  depth), color),
        VertexFormat(glm::vec3(width, -length,   depth), glm::vec3(width, 0,  depth), color),
        VertexFormat(glm::vec3(-width,  length,   depth), glm::vec3(width, 0, 0),  color),
        VertexFormat(glm::vec3(width,  length,   depth), glm::vec3(0, length, 0),  color),
        VertexFormat(glm::vec3(-width, -length, -depth), glm::vec3(width, length,  depth),  color),
        VertexFormat(glm::vec3(width, -length, -depth), glm::vec3(0, length,  depth), color),
        VertexFormat(glm::vec3(-width,  length, -depth), glm::vec3(width, length, 0),  color),
        VertexFormat(glm::vec3(width,  length, -depth), glm::vec3(0, 0,  depth),  color),
    };

    vector<unsigned int> indices =
    {
        0, 1, 2,        1, 3, 2,
        2, 3, 7,        2, 7, 6,
        1, 7, 3,        1, 5, 7,
        6, 7, 4,        7, 5, 4,
        0, 4, 1,        1, 4, 5,
        2, 6, 4,        0, 2, 4,
    };

    CreateMesh(mesh_name, vertices, indices);
}

void Tema2::CreateCube(float total_length, char* mesh_name, glm::vec3 color) {
    float length = total_length / 2;
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-length, -length,  length), glm::vec3(0, length, length), color),
        VertexFormat(glm::vec3(length, -length,  length), glm::vec3(length, 0, length), color),
        VertexFormat(glm::vec3(-length,  length,  length), glm::vec3(length, 0, 0),  color),
        VertexFormat(glm::vec3(length,  length,  length), glm::vec3(0, length, 0),  color),
        VertexFormat(glm::vec3(-length, -length, -length), glm::vec3(length, length, length),  color),
        VertexFormat(glm::vec3(length, -length, -length), glm::vec3(0, length, length), color),
        VertexFormat(glm::vec3(-length,  length, -length), glm::vec3(length, length, 0),  color),
        VertexFormat(glm::vec3(length,  length, -length), glm::vec3(0, 0, length),  color),
    };

    vector<unsigned int> indices =
    {
        0, 1, 2,        1, 3, 2,
        2, 3, 7,        2, 7, 6,
        1, 7, 3,        1, 5, 7,
        6, 7, 4,        7, 5, 4,
        0, 4, 1,        1, 4, 5,
        2, 6, 4,        0, 2, 4,
    };

    CreateMesh(mesh_name, vertices, indices);
}

void Tema2::InitPlayer()
{
    player= new MazePlayer();
    player->position = glm::vec3(0, 0, 0);
    CreateCube(player->length_cube, "fist", glm::vec3(1,1,0.65));
    CreateRectangular(player->length_cube, player->length_rectangle - player->length_cube, player->length_cube,"arm", glm::vec3(1, 0, 0));
    CreateRectangular(player->length_cube, player->length_rectangle, player->length_cube, "leg", glm::vec3(1, 0, 0));
    CreateRectangular(player->width_rectangle, player->length_rectangle, player->length_cube, "body", glm::vec3(0, 1, 0));
    CreateCube(player->length_cube, "head", glm::vec3(1,1,0.65));
}

void Tema2::Init()
{
    renderCameraTarget = false;

    camera = new Tema2Camera();

    InitPlayer();


    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    {
        Mesh* mesh = new Mesh("projectile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    projectile = new AttackProjectile();

    //initialize maze
    maze = new Maze();
    maze->generate_maze_map();
    maze->height = 5 * (double)player->length_rectangle;
    maze->length = 5 * (double)player->length_rectangle;
    CreateRectangular(maze->length, maze->height, maze->length, "maze_wall_piece", glm::vec3(1, 0, 0.5));
    CreateRectangular(maze->length, 0.001, maze->length, "maze_floor_piece", glm::vec3(0.5, 0, 1));

    //initialize enemies
    enemy = new MazeEnemy();
    enemy->set_limits(maze->length / 2, maze->length / 2);
    CreateRectangular(enemy->length, enemy->height, enemy->length, "enemy", glm::vec3(0, 0, 0.5));

    //set the player to the start position
    player->setStartPosition(maze);
    
    //set the camera to the start position
    thirdPersonCameraPosition[0] += player->position[0];
    thirdPersonCameraPosition[2] += player->position[2];
    thirdPersonCenter[0] += player->position[0];
    thirdPersonCenter[2] += player->position[2];

    this->firstPersonCameraPosition = glm::vec3(player->position[0], player->eyes_y, player->eyes_z + player->position[2]);
    //directia in care sa fie orientata camera este catre center, de accea z-ul centrului este mereu mai mic decat z-ul player-ului
    this->firstPersonCenter = glm::vec3(player->position[0], player->eyes_y, firstPersonCameraPosition[2] - distanceToCenter);
    this->dist_perspectives_xoz = thirdPersonCameraPosition[2] - firstPersonCameraPosition[2];
    this->dist_perspectives_oy = thirdPersonCameraPosition[1] - firstPersonCameraPosition[1];

    distanceToTargetFirstPersonCamera = -player->length_cube;

    //both position and the center have the same Y because the camera is oriented to center
    camera->Set(thirdPersonCameraPosition, thirdPersonCenter, glm::vec3(0, 1, 0));

    //initialize HUD
    hud = new Hud();
    Mesh* health = hud->CreateFullBar("health", glm::vec3(0, 0, 0), hud->lengthX, hud->lengthY, glm::vec3(1, 0, 0));
    Mesh* health_contour = hud->CreateBar("health_contour", glm::vec3(0, 0, 0), hud->lengthX, hud->lengthY, glm::vec3(1, 0, 0));
    Mesh* time_contour = hud->CreateBar("time_contour", glm::vec3(0, 0, 0), hud->lengthX, hud->lengthY, glm::vec3(0, 1, 1));
    Mesh* time = hud->CreateFullBar("time", glm::vec3(0, 0, 0), hud->lengthX, hud->lengthY, glm::vec3(0, 1, 1));
    AddMeshToList(health);
    AddMeshToList(health_contour);
    AddMeshToList(time_contour);
    AddMeshToList(time);

    //create deformed shader
     {
        Shader* shader = new Shader("MyDeformedShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "DeformedVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::DisplayPlayer() {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, player->position);
        modelMatrix *= transform3D::RotateOY(player->angle);
        modelMatrix = glm::translate(modelMatrix, player->left_fist);
        RenderMesh(meshes["fist"], shaders["VertexNormal"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, player->position);
        modelMatrix *= transform3D::RotateOY(player->angle);
        modelMatrix = glm::translate(modelMatrix, player->right_fist);
        RenderMesh(meshes["fist"], shaders["VertexNormal"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, player->position);
        modelMatrix *= transform3D::RotateOY(player->angle);
        modelMatrix = glm::translate(modelMatrix, player->left_arm);
        RenderMesh(meshes["arm"], shaders["VertexNormal"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1); 
        modelMatrix = glm::translate(modelMatrix, player->position);
        modelMatrix *= transform3D::RotateOY(player->angle);
        modelMatrix = glm::translate(modelMatrix, player->right_arm);
        RenderMesh(meshes["arm"], shaders["VertexNormal"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, player->position);
        modelMatrix *= transform3D::RotateOY(player->angle);
        modelMatrix = glm::translate(modelMatrix, player->left_leg);
        RenderMesh(meshes["leg"], shaders["VertexNormal"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, player->position);
        modelMatrix *= transform3D::RotateOY(player->angle);
        modelMatrix = glm::translate(modelMatrix, player->right_leg);
        RenderMesh(meshes["leg"], shaders["VertexNormal"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, player->position);
        modelMatrix *= transform3D::RotateOY(player->angle);
        modelMatrix = glm::translate(modelMatrix, player->body);
        RenderMesh(meshes["body"], shaders["VertexNormal"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, player->position);
        modelMatrix *= transform3D::RotateOY(player->angle);
        modelMatrix = glm::translate(modelMatrix, player->head);
        RenderMesh(meshes["head"], shaders["VertexNormal"], modelMatrix);
    }
    
}
void Tema2::DisplayProjectiles(float deltaTimeSeconds) {
    for (int i = 0; i < projectile->times.size();) {
        float distance = 3 * deltaTimeSeconds;
        projectile->times[i] -= distance;

        //calculare cu cat se modifica x si z in timp, y ramane la fel
        float ox_projection = sin(projectile->angles[i]) * distance;
        float oz_projection = cos(projectile->angles[i]) * distance;

        projectile->positions[i][0] -= ox_projection;
        projectile->positions[i][2] -= oz_projection;


        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, projectile->positions[i]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
        RenderMesh(meshes["projectile"], shaders["VertexNormal"], modelMatrix);

        if (projectile->times[i] < 0) {
            projectile->eliminate(i);
            i--;
        }
        i++;
    }

}

void Tema2::DisplayMaze() {
    
    for (int i = 0; i < maze->n; i++) {
        for (int j = 0; j < maze->n; j++) {
            if (maze->map[i][j] == 1) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(maze->length/2 + maze->length*i, maze->height/2, maze->length/2 + maze->length * j));
                RenderMesh(meshes["maze_wall_piece"], shaders["VertexNormal"], modelMatrix);
            }
            else {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(maze->length / 2 + maze->length * i, 0, maze->length / 2 + maze->length * j));
                RenderMesh(meshes["maze_floor_piece"], shaders["VertexNormal"], modelMatrix);
            }
        }
    }
}

void Tema2::DisplayEnemies(float deltaTime) {
    //display living enemies
    for (int i = 0; i < maze->n; i++) {
        for (int j = 0; j < maze->n; j++) {
            if (maze->map[i][j] == 2) {

                glm::mat4 modelMatrix = glm::mat4(1);
                //calculate the exact position on the map
                double ox = (double)enemy->position[0] + maze->length / 2 + maze->length * i;
                double oz = (double)enemy->position[2] + maze->length / 2 + maze->length * j;
                modelMatrix = glm::translate(modelMatrix, glm::vec3(ox, enemy->height / 2, oz));
                RenderMesh(meshes["enemy"], shaders["VertexNormal"], modelMatrix);
            }
        }
    }

    //display deformed enemies
    for (int i = 0; i < enemy->destroyed_time.size();) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(enemy->destroyed_x[i], enemy->height / 2, enemy->destroyed_z[i]));
        // deformez o sfera in loc de paralelipipedul ce reprezenta inamic-ul inainte de disturgere
        // deoarece sfera are mai multe vertex-uri si se va vede efectul shader-ului de deformare mai bine
        RenderDeformedMesh(meshes["projectile"], shaders["MyDeformedShader"], modelMatrix, glm::vec3(1,0.5,0));
        enemy->destroyed_time[i] -= 2*deltaTime;
        if (enemy->destroyed_time[i] <= 0) {
            enemy->eliminate(i);
        }
        else {
            i++;
        }
    }
}

inline glm::mat3 Translate(float translateX, float translateY)
{
    // TODO(student): Implement the translation matrix
    return glm::transpose(glm::mat3(1, 0, translateX, 0, 1, translateY, 0, 0, 1));

}

inline glm::mat3 Scale(float scaleX, float scaleY)
{
    // TODO(student): Implement the scaling matrix
    return glm::transpose(glm::mat3(scaleX, 0, 0, 0, scaleY, 0, 0, 0, 1));

}

void Tema2::DisplayHud() {

    auto camera2D = GetSceneCamera();

    glm::ivec2 resolution = window->GetResolution();
    //ortographic perspective
    camera2D->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 200);
    camera2D->SetRotation(glm::vec3(0, 0, 0));
    camera2D->Update();
    glClearColor(0.9, 1, 1, 1);
    glClear(GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    // sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    //find the coordinates of the healthbar and draw it
    float ox = resolution.x - hud->lengthX - 20;
    float oy = resolution.y - 2*hud->lengthY;
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(ox, oy);
    modelMatrix *= Scale(1-hud->damageTaken, 1);
    RenderMesh2D(meshes["health"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(ox, oy);
    RenderMesh2D(meshes["health_contour"], shaders["VertexColor"], modelMatrix);

    //find the coordinates of the remaining time bar and draw it
    ox = resolution.x - hud->lengthX - 20;
    oy = resolution.y - 3 * hud->lengthY - hud->lengthY / 2;
    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(ox, oy);
    modelMatrix *= Scale(hud->remainingTime, 1);
    RenderMesh2D(meshes["time"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    modelMatrix *= Translate(ox, oy);
    RenderMesh2D(meshes["time_contour"], shaders["VertexColor"], modelMatrix);
}

void Tema2::Update(float deltaTimeSeconds)
{
    DisplayHud();
    DisplayMaze();
    DisplayProjectiles(deltaTimeSeconds);
    enemy->move(deltaTimeSeconds);
    DisplayEnemies(deltaTimeSeconds);
    DisplayPlayer();

    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }

    //if the game is won stop the time
    if (!won_game) {
        won_game = player->checkReachedEnd(maze);
        if (won_game) {
            cout << "YOU WON !!!";
        }
        hud->remainingTime -= 0.01 * deltaTimeSeconds;
    }

    if (hud->remainingTime <= 0) {
        cout << "You FAILED !!!";
        exit(0);
    }

    //if the health of the player falls to much lose the game
    if (player->checkEnemyCollision(enemy, maze)) {
        hud->damageTaken += 0.25;
        if (hud->damageTaken >= 1.f) {
            cout << "You Lost !!!";
            exit(0);
        }
    }

    //check collisions between the projectiles and the enemies
    for (int i = 0; i < projectile->positions.size(); ) {
        
        if (projectile->checkCollisionMaze(i, maze)) {
            projectile->eliminate(i);
        }
        else
            if (projectile->checkCollisionEnemy(i, enemy, maze)) {
                projectile->eliminate(i);
            }
            else i++;
        }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderDeformedMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    //send color parameter to the vertex
    GLint location = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(location, 1, glm::value_ptr(color));

    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{   
    if (window->KeyHold(GLFW_KEY_W)) {
        //when the player moves in a direction we have to calculate how much the coordinates
        //are changing because he is not always walking sttraight on
        //the coordinats, he can walk on a certain angle on XoZ
        player->moveForward(deltaTime);
        camera->TranslateForward(2 * deltaTime);

        //if collision go back
        if (player->checkWallCollision(maze)) {
            player->moveForward(-deltaTime);
            camera->TranslateForward(-2 * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        player->moveBackward(deltaTime);
        camera->TranslateForward(-2 * deltaTime);
        
        //if collision go back
        if (player->checkWallCollision(maze)) {
            player->moveBackward(-deltaTime);
            camera->TranslateForward(+2 * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        player->moveLeft(deltaTime);
        camera->TranslateRight(-2 * deltaTime);
        
        //if collision go back
        if (player->checkWallCollision(maze)) {
            player->moveLeft(-deltaTime);
            camera->TranslateRight(+2 * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        player->moveRight(deltaTime);
        camera->TranslateRight(2 * deltaTime);
    
        //if collision go back
        if (player->checkWallCollision(maze)) {
            player->moveRight(-deltaTime);
            camera->TranslateRight(-2 * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_N)) {
        //rotate towards left
        player->angle += deltaTime;
        if (isThirdPersonCamera == true) {
            camera->RotateThirdPerson_OY(deltaTime);
        }
        else {//folosim tot rotire third person caci altfel player-ul loveste camera
            camera->RotateThirdPerson_OY(deltaTime);
        }
    }
    if (window->KeyHold(GLFW_KEY_M)) {
        //rotate towards left
        player->angle -= deltaTime;
        if (isThirdPersonCamera == true) {
            camera->RotateThirdPerson_OY(-deltaTime);
        }
        else {
            camera->RotateThirdPerson_OY(-deltaTime);
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    
    //lansare proiectil
    if (key == GLFW_KEY_SPACE && isInAttackMode == true) {
        projectile->angles.push_back(player->angle);
        projectile->times.push_back(projectile->life_span);
        projectile->positions.push_back(camera->position);
    }

    //intrare in mod attack si first person view
    if (key == GLFW_KEY_RIGHT_CONTROL) {
        if (isThirdPersonCamera == true) {
            //gasire pozitie pt first person camera
            glm::vec3 new_position = camera->position;
            //calc distante pe xoz
            // - because the direction to forward is to (-inf,-inf) in xoz sa it's the opposite for positive values
            float ox_projection = -sin(player->angle) * dist_perspectives_xoz;
            float oz_projection = -cos(player->angle) * dist_perspectives_xoz;
            //add to XoZ
            new_position[2] += oz_projection;
            new_position[0] += ox_projection;
            //ad to Y
            new_position[1] -= dist_perspectives_oy;

            camera->SetOnlyPosition(new_position);
            isThirdPersonCamera = false;
            isInAttackMode = true;

            //distanta la target fata de care se face rotirea se schimba
            camera->distanceToTarget = distanceToTargetFirstPersonCamera;
        }
        else {
            //gasire pozitie pt third person camera
            glm::vec3 new_position = camera->position;
            float ox_projection = sin(player->angle) * dist_perspectives_xoz;
            float oz_projection = cos(player->angle) * dist_perspectives_xoz;
            //add to XoZ
            new_position[2] += oz_projection;
            new_position[0] += ox_projection;
            //ad to Y
            new_position[1] += dist_perspectives_oy;
            
            camera->SetOnlyPosition(new_position);
            isThirdPersonCamera = true;
            isInAttackMode = false;

            //distanta la target fata de care se face rotirea se schimba
            camera->distanceToTarget = distanceToTargetThirdPersonCamera;
        }
        
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
			camera->RotateThirdPerson_OY(-sensivityOY * deltaX);

        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
