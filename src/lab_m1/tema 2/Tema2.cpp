#include "lab_m1/tema 2/Tema2.h"

using namespace m1;

Tema2::Tema2() { }

Tema2::~Tema2() { }

void Tema2::Init() {
    GetCameraInput()->SetActive(false);
    window->DisablePointer();

    tileMap = new TileMap(13, 13);

    player = new Player(tileMap->startPos.first, tileMap->startPos.second, meshes, mapTextures,
        PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 2", "Textures"),
        PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 2", "Textures", "shotgunTexture.png"));

    camera = new MyCamera(player->getHitbox()->pos, window->props.aspectRatio);

    PlaceEnemies();

    createBasicRectMesh();

    {
        Mesh* cube = new Mesh("cube");
        cube->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[cube->GetMeshID()] = cube;
    }

    {
        Mesh* sphere = new Mesh("sphere");
        sphere->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[sphere->GetMeshID()] = sphere;
    }

    {
        Shader* shader = new Shader("textureShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 2", "Shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 2", "Shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("enemyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 2", "Shaders", "EnemyVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 2", "Shaders", "EnemyFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("basic2DShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 2", "Shaders", "BasicVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 2", "Shaders", "BasicFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 2", "Textures", "brickWall.png").c_str(), GL_REPEAT);
        mapTextures["brickWall"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 2", "Textures", "asphalt.png").c_str(), GL_REPEAT);
        mapTextures["asphalt"] = texture;
    }
}

void Tema2::FrameStart() {
    glClearColor(0.078f, 0.094f, 0.321f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds) {
    player->handleInput(deltaTimeSeconds, window, camera, bullets, tileMap);
    
    MoveBullets(deltaTimeSeconds);
    MoveEnemies(deltaTimeSeconds);
    CheckBulletEnemyCollisions();
    CheckBulletWorldCollisons();
    CheckPlayerEnemyCollision();
    CheckIfPlayerEscaped();

    RenderMapBounds();
    RenderWalls();
    RenderEnemies(deltaTimeSeconds);
    RenderBullets();
    RenderPlayer();
    RenderContainers();

    player->getTimer()->updateValue(-deltaTimeSeconds);
    if (player->getTimer()->isEmpty()) {
        Exit();
    }
}

void Tema2::OnInputUpdate(float deltaTimeSeconds, int mods) {
    
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    camera->Rotate_OX(deltaY);
    camera->Rotate_OY(deltaX);
}

void Tema2::createBasicRectMesh() {
    {
        std::vector<VertexFormat> v = {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(1, 0, 1), glm::vec3(0), glm::vec3(0))
        };

        std::vector<unsigned int> i = {
            0, 1, 2,
            1, 3, 2
        };

        Mesh* rect = new Mesh("horizontalRect");
        rect->SetDrawMode(GL_TRIANGLES);
        rect->InitFromData(v, i);
        meshes[rect->GetMeshID()] = rect;
    }

    {
        std::vector<VertexFormat> v = {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0), glm::vec3(0))
        };

        std::vector<unsigned int> i = {
            0, 1, 2,
            1, 3, 2
        };

        Mesh* rect = new Mesh("verticalRect");
        rect->SetDrawMode(GL_TRIANGLES);
        rect->InitFromData(v, i);
        meshes[rect->GetMeshID()] = rect;
    }
}

void Tema2::RenderMapBounds() {
    glm::mat4 modelMatrix(1);

    glm::vec3 scale(tileMap->getWorldLength(), 1, tileMap->getWorldWidth());
    modelMatrix = glm::scale(modelMatrix, scale);

    RenderMeshWithTexture(meshes["horizontalRect"], mapTextures["asphalt"], modelMatrix);
}

void Tema2::RenderWalls() {
    auto layout = tileMap->getLayout();

    for (int i = 0; i < layout.size(); i++) {
        for (int j = 0; j < layout[i].size(); j++) {
            if (layout[i][j]->value == 1) {
                glm::mat4 modelMatrix(1);

                glm::vec3 scale(layout[i][j]->hitbox->dimensions);
                glm::vec3 translate(layout[i][j]->hitbox->pos.x + tileMap->tileW / 2, 5, layout[i][j]->hitbox->pos.z + tileMap->tileL / 2);

                modelMatrix = glm::translate(modelMatrix, translate);
                modelMatrix = glm::scale(modelMatrix, scale);

                RenderMeshWithTexture(meshes["cube"], mapTextures["brickWall"], modelMatrix);
            }
        }
    }
}

void Tema2::RenderPlayer() {
    {

        if (camera->isInThirdPerson()) {
            auto bodyParts = player->getBodyParts();

            for (int i = 0; i < bodyParts.size(); i++) {
                glm::mat4 modelMatrix(1);

                auto part = bodyParts.at(i);

                glm::vec3 forward = camera->getForward();
                float ro = atan2(forward.x, forward.z);

                modelMatrix = glm::translate(modelMatrix, player->getHitbox()->pos);
                modelMatrix = glm::rotate(modelMatrix, ro, glm::vec3(0, 1, 0));
                modelMatrix = glm::translate(modelMatrix, part.first->pos);
                modelMatrix = glm::scale(modelMatrix, part.first->dimensions);

                shaders["Color"]->Use();
                glUniformMatrix4fv(shaders["Color"]->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
                glUniformMatrix4fv(shaders["Color"]->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
                glUniformMatrix4fv(shaders["Color"]->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
                
                glUniform3fv(shaders["Color"]->GetUniformLocation("color"), 1, glm::value_ptr(part.second));

                meshes["cube"]->Render();
            }
        }
    }

    {
        glm::mat4 modelMatrix(1);

        glm::vec3 forward = camera->getForward();

        float ro = atan2(forward.x, forward.z);

        modelMatrix = glm::translate(modelMatrix, camera->getPosition());
        modelMatrix = glm::rotate(modelMatrix, ro - RADIANS(270), glm::vec3(0, 1, 0));
        if (camera->isInThirdPerson()) {
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -1.f, -0.5f));
        }
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -1.f, -0.2f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2));

        RenderMeshWithTexture(meshes["shotgun"], mapTextures["shotgun"], modelMatrix);
    }
}

void Tema2::RenderContainers() {
    {
        auto health = player->getHealth();

        glm::mat4 modelMatrix(1.f);

        modelMatrix = glm::translate(modelMatrix, health->getHitbox()->pos);
        modelMatrix = glm::scale(modelMatrix, health->getHitbox()->dimensions);

        shaders["basic2DShader"]->Use();

        glUniformMatrix4fv(shaders["basic2DShader"]->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(glm::ortho(0.f, 12.8f, 0.f, 7.2f, -10.f, 10.f)));
        glUniformMatrix4fv(shaders["basic2DShader"]->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform3fv(shaders["basic2DShader"]->GetUniformLocation("color"), 1, glm::value_ptr(health->getColor()));

        meshes["verticalRect"]->Render();
    

        modelMatrix = glm::mat4(1.f);
        modelMatrix = glm::translate(modelMatrix, health->getHitbox()->pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(health->getMaxWidth(), health->getHitbox()->dimensions.y, 0));

        glUniformMatrix4fv(shaders["basic2DShader"]->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform3fv(shaders["basic2DShader"]->GetUniformLocation("color"), 1, glm::value_ptr(glm::vec3(0.2)));
        meshes["verticalRect"]->Render();
    }

    {
        auto timer = player->getTimer();

        glm::mat4 modelMatrix(1.f);

        modelMatrix = glm::translate(modelMatrix, timer->getHitbox()->pos);
        modelMatrix = glm::scale(modelMatrix, timer->getHitbox()->dimensions);

        shaders["basic2DShader"]->Use();

        glUniformMatrix4fv(shaders["basic2DShader"]->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(glm::ortho(0.f, 12.8f, 0.f, 7.2f, -10.f, 10.f)));
        glUniformMatrix4fv(shaders["basic2DShader"]->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform3fv(shaders["basic2DShader"]->GetUniformLocation("color"), 1, glm::value_ptr(timer->getColor()));
        meshes["verticalRect"]->Render();


        modelMatrix = glm::mat4(1.f);
        modelMatrix = glm::translate(modelMatrix, timer->getHitbox()->pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(timer->getMaxWidth(), timer->getHitbox()->dimensions.y, 0));

        glUniformMatrix4fv(shaders["basic2DShader"]->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform3fv(shaders["basic2DShader"]->GetUniformLocation("color"), 1, glm::value_ptr(glm::vec3(0.2)));
        meshes["verticalRect"]->Render();
    }
}

void Tema2::MoveBullets(float deltaTimeSeconds) {
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i]->move(deltaTimeSeconds);

        if (bullets[i]->hasExpired()) {
            auto bullet = bullets[i];
            bullets.erase(bullets.begin() + i);
            delete bullet;

            i--;
        }
    }
}

void Tema2::RenderBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        glm::mat4 modelMatrix(1);

        modelMatrix = glm::translate(modelMatrix, bullets[i]->getPos());
        modelMatrix = glm::scale(modelMatrix, bullets[i]->getDimensions());

        shaders["VertexColor"]->Use();
        glUniformMatrix4fv(shaders["VertexColor"]->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shaders["VertexColor"]->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniformMatrix4fv(shaders["VertexColor"]->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        meshes["cube"]->Render();
    }
}

void Tema2::CheckBulletEnemyCollisions() {
    for (int i = 0; i < bullets.size(); i++) {
        for (int j = 0; j < enemies.size(); j++) {
            auto b = bullets[i]->getHitbox();
            auto e = enemies[j]->getHitbox();

            if (b->pos.x + b->dimensions.x >= e->pos.x - 1.5f && b->pos.x + b->dimensions.x <= e->pos.x + e->dimensions.x - 1.5f
                && b->pos.z + b->dimensions.z >= e->pos.z - 1.5f && b->pos.z + b->dimensions.z <= e->pos.z + e->dimensions.z - 1.5f
                && b->pos.y + b->dimensions.y >= e->pos.y - 1.5f && b->pos.y + b->dimensions.y <= e->pos.y + e->dimensions.y - 1.5f) {

                auto bullet = bullets[i];
                bullets.erase(bullets.begin() + i);
                delete bullet;
                i--;

                enemies[j]->startDeathAnimation();

                break;
            }
        }
    }
}

void Tema2::CheckBulletWorldCollisons() {
    auto layout = tileMap->getLayout();
    for (int i = 0; i < bullets.size(); i++) {
        bool collision = false;
        for (int j = 0; j < layout.size(); j++) {
            for (int k = 0; k < layout[j].size(); k++) {
                if (layout[j][k]->value == 1) {
                    auto b = bullets[i]->getHitbox();
                    auto w = layout[j][k]->hitbox;

                    if (b->pos.x + b->dimensions.x >= w->pos.x && b->pos.x + b->dimensions.x <= w->pos.x + w->dimensions.x
                        && b->pos.z + b->dimensions.z >= w->pos.z && b->pos.z + b->dimensions.z <= w->pos.z + w->dimensions.z) {

                        auto bullet = bullets[i];
                        bullets.erase(bullets.begin() + i);
                        delete bullet;
                        i--;

                        collision = true;
                        break;
                    }
                }
            }

            if (collision) {
                break;
            }
        }
    }
}

void Tema2::CheckPlayerEnemyCollision() {
    auto p = player->getHitbox();

    for (int i = 0; i < enemies.size(); i++) {
        auto e = enemies[i]->getHitbox();

        if (p->pos.x + p->dimensions.x / 2 >= e->pos.x - 1.5f && p->pos.x + p->dimensions.x / 2 <= e->pos.x + e->dimensions.x + 1.5f
            && p->pos.z + p->dimensions.z / 2 >= e->pos.z - 1.5f && p->pos.z + p->dimensions.z / 2 <= e->pos.z + e->dimensions.z + 1.5f) {

            player->getHealth()->updateValue(-1.f);
            if (player->getHealth()->isEmpty()) {
                Exit();
            }
        }
    }
}

void Tema2::CheckIfPlayerEscaped() {
    auto endPos = tileMap->getLayout().at(tileMap->endPos.first).at(tileMap->endPos.second)->hitbox;
    auto p = player->getHitbox();

    if (p->pos.x >= endPos->pos.x && p->pos.x <= endPos->pos.x + endPos->dimensions.x * 3 / 2
        && p->pos.z >= endPos->pos.z && p->pos.z <= endPos->pos.z + endPos->dimensions.z * 3 / 2) {
        Exit();
    }
}

void Tema2::PlaceEnemies() {
    auto layout = tileMap->getLayout();
    for (int i = 0; i < layout.size(); i++) {
        for (int j = 0; j < layout[i].size(); j++) {
            if (layout[i][j]->value == 2) {
                enemies.push_back(new EnemyTema2(glm::vec3(i * tileMap->tileW + tileMap->tileW / 2, 1.5f, j * tileMap->tileL + tileMap->tileL / 2), glm::vec3(3.f)));
            }
        }
    }
}

void Tema2::RenderEnemies(float deltaTimeSeconds) {
    for (int i = 0; i < enemies.size(); i++) {

        glm::mat4 modelMatrix(1.f);

        modelMatrix = glm::translate(modelMatrix, enemies[i]->getHitbox()->pos);
        modelMatrix = glm::scale(modelMatrix, enemies[i]->getHitbox()->dimensions);

        shaders["enemyShader"]->Use();
        glUniformMatrix4fv(shaders["enemyShader"]->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shaders["enemyShader"]->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniformMatrix4fv(shaders["enemyShader"]->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

        glUniform1i(glGetUniformLocation(shaders["enemyShader"]->program, "death_stage"), enemies[i]->getDeathStage());
        glUniform1f(glGetUniformLocation(shaders["enemyShader"]->program, "time"), Engine::GetElapsedTime());

        meshes["sphere"]->Render();
    }
}

void Tema2::MoveEnemies(float deltaTimeSeconds) {
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i]->getDeathStage() == 2) {
            auto e = enemies[i];
            enemies.erase(enemies.begin() + i);
            delete e;
        } else {
            enemies[i]->Move(deltaTimeSeconds);
        }
    }
}


void Tema2::RenderMeshWithTexture(Mesh* mesh, Texture2D* texture, glm::mat4& modelMatrix) {
    shaders["textureShader"]->Use();
    glUniformMatrix4fv(shaders["textureShader"]->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shaders["textureShader"]->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
    glUniformMatrix4fv(shaders["textureShader"]->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glUniform3fv(glGetUniformLocation(shaders["textureShader"]->program, "light_position"), 1, glm::value_ptr(camera->getPosition()));
    glUniform3fv(glGetUniformLocation(shaders["textureShader"]->program, "eye_position"), 1, glm::value_ptr(camera->getPosition()));
    glUniform3fv(glGetUniformLocation(shaders["textureShader"]->program, "light_direction"), 1, glm::value_ptr(camera->getForward()));
    glUniform1f(glGetUniformLocation(shaders["textureShader"]->program, "material_kd"), 0.5f);
    glUniform1f(glGetUniformLocation(shaders["textureShader"]->program, "material_ks"), 1.5f);
    glUniform1i(glGetUniformLocation(shaders["textureShader"]->program, "material_shininess"), 30);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    glUniform1i(shaders["textureShader"]->loc_textures[0], 0);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}