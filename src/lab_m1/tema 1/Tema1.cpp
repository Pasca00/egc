#include "lab_m1/tema 1/Tema1.h"
#include "lab_m1/tema 1/Transform2D.cpp"
#include "lab_m1/tema 1/Map/Map.h"

using namespace m1;

Tema1::Tema1() : logicSpace(0, 0, 0, 0) { }

Tema1::~Tema1() { }

void Tema1::Init() {
    srand(time(NULL));

    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 1));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    //map = new WorldMap(2000, 1500, meshes);

    player = new Entity(meshes);

    logicSpace.w = 600;
    logicSpace.h = 600;
    logicSpace.x = player->getBounds().x - logicSpace.w / 2;
    logicSpace.y = player->getBounds().y - logicSpace.h / 2;

    Enemy::createMesh(meshes);

    enemySpawnAccum = 0;
    enemySpawnRate = 3;
}

void Tema1::Update(float deltaTimeSeconds) {
    checkCollisions();

    Rect playerBounds = player->getBounds();
    glm::mat3 modelMatrix(1);
    glm::ivec2 resolution = window->GetResolution();

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0, 0, 0), false);

    glm::mat3 visMatrix(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    RenderHealthbar();

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= Transform::Translate(player->getBounds().x, player->getBounds().y);
    modelMatrix *= Transform::Rotate(player->rotationAngle);
    RenderMesh2D(meshes["playerMesh"], shaders["VertexColor"], modelMatrix);

    RenderProjectiles(visMatrix);

    RenderEnemies(visMatrix);

    RenderObjects(visMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    modelMatrix *= Transform::Translate(0, 0);
    RenderMesh2D(meshes["worldMap"], shaders["VertexColor"], modelMatrix);

    if (player->isDead()) {
        system("cls");
        printf("Game over!\nFinal score: %d\n\n", player->getScore());
        window->Close();
    }
}


void Tema1::RenderEnemies(glm::mat3& visMatrix) {
    glm::mat3 modelMatrix;

    for (auto e : enemies) {
        e->computeRotationAngle();

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= Transform::Translate(e->getBounds().x + 50, e->getBounds().y + 50);
        modelMatrix *= Transform::Rotate(e->rotationAngle);
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderProjectiles(glm::mat3& visMatrix) {
    glm::mat3 modelMatrix;

    for (auto p : projectiles) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= Transform::Translate(p->getBounds().x, p->getBounds().y);
        modelMatrix *= Transform::Rotate(p->rotationAngle);
        modelMatrix *= Transform::Scale(p->getBounds().w, p->getBounds().h);
        RenderMesh2D(meshes["rect"], modelMatrix, glm::vec3(1));
    }
}

void Tema1::RenderObjects(glm::mat3& visMatrix) {
    /*
    std::vector<WorldObject*> objects = map->getObjects();
    glm::mat3 modelMatrix;

    for (auto it = objects.begin(); it != objects.end(); it++) {
        Rect bounds = (*it)->bounds;

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;
        modelMatrix *= Transform::Translate(bounds.x, bounds.y);
        modelMatrix *= Transform::Scale(bounds.w, bounds.h);
        RenderMesh2D(meshes["rect"], modelMatrix, (*it)->color);
    }
    */
}

void Tema1::RenderHealthbar() {
    glm::mat3 modelMatrix(1);
    RenderMesh2D(meshes["healthbarContainer"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= Transform::Translate(player->healthbar->bounds.x, player->healthbar->bounds.y);
    modelMatrix *= Transform::Scale(player->healthbar->bounds.w, player->healthbar->bounds.h);
    RenderMesh2D(meshes["rect"], modelMatrix, player->healthbar->currentColor);
}

void Tema1::spawnEnemy(float deltaTime) {
    enemySpawnAccum += deltaTime;
    if (enemySpawnAccum >= enemySpawnRate) {
        enemies.push_back(new Enemy(player, map->bounds.width, map->bounds.height));
        enemySpawnAccum = 0;
    }
}

void Tema1::moveProjectiles(float deltaTimeSeconds) {
    for (auto p : projectiles) {
        p->move(deltaTimeSeconds);
    }
}

void Tema1::moveEnemies(float deltaTimeSeconds) {
    for (auto e : enemies) {
        e->moveToPlayer(deltaTimeSeconds);
    }
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear) {
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

glm::mat3 Tema1::VisualizationTransf2DUnif(const Rect& logicSpace, const ViewportSpace& viewSpace) {
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.w;
    sy = viewSpace.height / logicSpace.h;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.w) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.h) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::FrameStart() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema1::OnInputUpdate(float deltaTimeSeconds, int mods) {
    player->handleInput(window, logicSpace, projectiles, deltaTimeSeconds, mods);
    
    spawnEnemy(deltaTimeSeconds);
    moveProjectiles(deltaTimeSeconds);
    moveEnemies(deltaTimeSeconds);
}

void Tema1::checkCollisions() {
    checkPlayerWorldCollisions();
    checkPlayerEnemyCollisions();
    checkProjectilesEnemyCollisions();
    checkProjectilesWorldCollisions();
}

void Tema1::checkPlayerWorldCollisions() {
    /*
    player->clearCollisions();

    std::vector<WorldObject*> objects = map->getObjects();
    Rect bounds = player->getBounds();
    float playerSpeed = player->getMoveSpeed();

    if (bounds.x - bounds.w < map->bounds.x) {
        player->setLeftCollision(true);
    }

    if (bounds.y - bounds.h < map->bounds.y) {
        player->setDownCollision(true);
    }

    if (bounds.x + bounds.w > map->bounds.x + map->bounds.width) {
        player->setRightCollision(true);
    }

    if (bounds.y + bounds.h > map->bounds.y + map->bounds.height) {
        player->setUpCollision(true);
    }

    float offset = 10;

    for (auto o : objects) {
        if (player->xDirection == X_DIR_LEFT) {
            if (o->bounds.x + o->bounds.w >= bounds.x - bounds.w && bounds.x - bounds.w >= o->bounds.x
                && ((bounds.y + bounds.h - offset >= o->bounds.y && bounds.y - bounds.h + offset <= o->bounds.y + o->bounds.h))) {
                player->setLeftCollision(true);
            }
        }

        if (player->xDirection == X_DIR_RIGHT) {
            if (bounds.x + bounds.w >= o->bounds.x && bounds.x + bounds.w <= o->bounds.x + o->bounds.w
                && ((bounds.y + bounds.h - offset >= o->bounds.y && bounds.y - bounds.h + offset <= o->bounds.y + o->bounds.h))) {
                player->setRightCollision(true);
            }
        }

        if (player->yDirection == Y_DIR_DOWN) {
            if (o->bounds.y + o->bounds.h >= bounds.y - bounds.h && bounds.y - bounds.h >= o->bounds.y
                && ((bounds.x + bounds.w - offset >= o->bounds.x && bounds.x - bounds.w + offset <= o->bounds.x + o->bounds.w))) {
                player->setDownCollision(true);
            }
        }

        if (player->yDirection == Y_DIR_UP) {
            if (bounds.y + bounds.h >= o->bounds.y && bounds.y + bounds.h <= o->bounds.y + o->bounds.h
                && ((bounds.x + bounds.w - offset >= o->bounds.x && bounds.x - bounds.w + offset <= o->bounds.x + o->bounds.w))) {
                player->setUpCollision(true);
            }
        }
    }
    */
}

void Tema1::checkPlayerEnemyCollisions() {
    Rect bounds = player->getBounds();

    for (int i = 0; i < enemies.size(); i++) {
        auto e = enemies[i];
        if (bounds.x + bounds.w >= e->getBounds().x && bounds.x + bounds.w <= e->getBounds().x + e->getBounds().w
            && bounds.y + bounds.h >= e->getBounds().y && bounds.y + bounds.h <= e->getBounds().y + e->getBounds().h) {
            player->handleCollision(enemies[i]);
            enemies.erase(enemies.begin() + i);
            e->handleCollision(player);

            i--;
        }
    }
}

void Tema1::checkProjectilesEnemyCollisions() {
    for (int i = 0; i < projectiles.size(); i++) {
        for (int j = 0; j < enemies.size(); j++) {
            auto p = projectiles[i];
            auto e = enemies[j];
            if (p->getBounds().x + p->getBounds().w >= e->getBounds().x && p->getBounds().x + p->getBounds().w <= e->getBounds().x + e->getBounds().w
                && p->getBounds().y + p->getBounds().h >= e->getBounds().y && p->getBounds().y + p->getBounds().h <= e->getBounds().y + e->getBounds().h) {
                
                projectiles.erase(projectiles.begin() + i);
                enemies.erase(enemies.begin() + j);

                p->handleCollision(e);

                i--;
                j--;

                break;
            }
        }
    }
}

void Tema1::checkProjectilesWorldCollisions() {
    /*
    for (int i = 0; i < projectiles.size(); i++) {
        bool destroyed = false;
        auto p = projectiles[i];
        auto bounds = p->getBounds();

        if (bounds.x <= map->bounds.x) {
            destroyed = true;
        } else if (bounds.x + bounds.w >= map->bounds.width) {
            destroyed = true;
        } else if (bounds.y <= map->bounds.y) {
            destroyed = true;
        } else if (bounds.y + bounds.h >= map->bounds.height) {
            destroyed = true;
        } else {
            for (auto o : map->getObjects()) {
                if (bounds.x + bounds.w >= o->bounds.x && bounds.x + bounds.w <= o->bounds.x + o->bounds.w
                    && bounds.y + bounds.h >= o->bounds.y && bounds.y + bounds.h <= o->bounds.y + o->bounds.h) {
                    
                    destroyed = true;
                    break;
                }
            }
        }

        if (destroyed) {
            projectiles.erase(projectiles.begin() + i);
            delete p;

            i--;
        }
    }
    */
}