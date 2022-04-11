#include "lab_m1/tema 3/Tema3.h"

using namespace m1;

Tema3::Tema3() { }

Tema3::~Tema3() { }

void Tema3::Init() {
    CreateFloor();
    CreateWalls();
    CreateSpotLights();
    CreateDancers();

    globe = new Globe(glm::vec3(floor[4][4]->pos.x, height - 1.f, floor[4][4]->pos.z));

    {
        Shader* textureShader = new Shader("globe");
        textureShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 3", "Shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        textureShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema 3", "Shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        textureShader->CreateAndLink();
        shaders[textureShader->GetName()] = textureShader;
    }

    {
        Mesh* globeMesh = new Mesh("globe");
        globeMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[globeMesh->GetMeshID()] = globeMesh;
    }

    {
        Mesh* dancer = new Mesh("dancer");
        dancer->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[dancer->GetMeshID()] = dancer;
    }

    {
        std::vector<VertexFormat> v = {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(1, 0, 1), glm::vec3(0), glm::vec3(0))
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            1, 3, 2
        };

        Mesh* rect = new Mesh("xOz_rect");
        rect->SetDrawMode(GL_TRIANGLES);
        rect->InitFromData(v, indices);
        meshes[rect->GetMeshID()] = rect;
    }

    {
        std::vector<VertexFormat> v = {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0), glm::vec3(0))
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            1, 3, 2
        };

        Mesh* rect = new Mesh("xOy_rect");
        rect->SetDrawMode(GL_TRIANGLES);
        rect->InitFromData(v, indices);
        meshes[rect->GetMeshID()] = rect;
    }

    {
        std::vector<VertexFormat> v = {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(0)),
            VertexFormat(glm::vec3(0, 1, 1), glm::vec3(0), glm::vec3(0))
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            1, 3, 2
        };

        Mesh* rect = new Mesh("zOy_rect");
        rect->SetDrawMode(GL_TRIANGLES);
        rect->InitFromData(v, indices);
        meshes[rect->GetMeshID()] = rect;
    }

    {
        std::vector<VertexFormat> v;
        std::vector<unsigned int> i;

        v.push_back(VertexFormat(glm::vec3(0, 0, 0)));

        float t = 0;
        for (int j = 0; j < 60; j++) {
            v.push_back(VertexFormat(glm::vec3(cos(t), -1, sin(t))));
            i.push_back(0);
            i.push_back(j + 1);
            i.push_back(j + 2);

            t += 360 / 60;
        }

        Mesh* cone = new Mesh("cone");
        cone->SetDrawMode(GL_TRIANGLES);
        cone->InitFromData(v, i);
        meshes[cone->GetMeshID()] = cone;
    }
}

void Tema3::FrameStart() {
    glClearColor(0.078f, 0.094f, 0.321f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds) {
    UpdateDancers(deltaTimeSeconds);

    DrawFloor();
    DrawCeiling();
    DrawWalls();
    DrawGlobe();
    DrawSpotLights();
    DrawDancers();
}

void Tema3::CreateFloor() {
    floorIsActive = true;
    floor = std::vector< std::vector<LightCell*> >(gridH, std::vector<LightCell*>(gridW, NULL));
    for (int i = 0; i < gridH; i++) {
        for (int j = 0; j < gridW; j++) {
            floor[i][j] = new LightCell(i, j, 0);
        }
    }
}

void Tema3::CreateWalls() {
    auto cellSize = floor[0][0]->size.x;
    for (int i = 0; i < 8; i++) {
        zOyColumsClose.push_back(new Column(glm::vec3(0, 0, i * cellSize), glm::vec3(0, height, cellSize), floor[0][i]));
        zOyColumsFar.push_back(new Column(glm::vec3(width, 0, i * cellSize), glm::vec3(0, height, cellSize), floor[7][i]));
        xOyColumsClose.push_back(new Column(glm::vec3(i * cellSize, 0, 0), glm::vec3(cellSize, height, 0), floor[i][0]));
        xOyColumsFar.push_back(new Column(glm::vec3(i * cellSize, 0, width), glm::vec3(cellSize, height, 0), floor[i][7]));
    }
}

void Tema3::CreateSpotLights() {
    spotlights.push_back(new Spotlight(glm::vec3(floor[2][2]->lightPos.x, height, floor[2][2]->lightPos.z)));
    spotlights.push_back(new Spotlight(glm::vec3(floor[2][5]->lightPos.x, height, floor[2][5]->lightPos.z)));
    spotlights.push_back(new Spotlight(glm::vec3(floor[5][2]->lightPos.x, height, floor[5][2]->lightPos.z)));
    spotlights.push_back(new Spotlight(glm::vec3(floor[5][5]->lightPos.x, height, floor[5][5]->lightPos.z)));

    spotlightsAreActive = true;
}

void Tema3::CreateDancers() {
    int numberOfDancers = 6;
    for (int i = 0; i < numberOfDancers; i++) {
        int j = rand() % 5 + 2;
        int k = rand() % 5 + 2;
        dancers.push_back(new Dancer(Dancer::movementOptions.at(rand() % Dancer::movementOptions.size()), floor[j][k]->lightPos));
    }
}

void Tema3::DrawFloor() {
    shaders["globe"]->Use();
    glUniform1i(shaders["globe"]->GetUniformLocation("isCell"), 1);

    for (int i = 0; i < floor.size(); i++) {
        for (int j = 0; j < floor[i].size(); j++) {
            glm::mat4 modelMatrix(1.f);

            modelMatrix = glm::translate(modelMatrix, floor[i][j]->pos);
            modelMatrix = glm::scale(modelMatrix, floor[i][j]->size);

            shaders["globe"]->Use();

            glUniform3fv(shaders["globe"]->GetUniformLocation("cellColor"), 1, glm::value_ptr(floor[i][j]->color));
            glUniform3fv(shaders["globe"]->GetUniformLocation("cellLightPosition"), 1, glm::value_ptr(floor[i][j]->lightPos));

            DrawMeshWithTexture(modelMatrix, meshes["xOz_rect"]);
        }
    }

    glUniform1i(shaders["globe"]->GetUniformLocation("isCell"), 0);
}

void Tema3::DrawWalls() {
    shaders["globe"]->Use();
    glUniform1i(shaders["globe"]->GetUniformLocation("isColumn"), 1);

    for (int i = 0; i < zOyColumsClose.size(); i++) {
        glm::mat4 modelMatrix(1.f);

        modelMatrix = glm::translate(modelMatrix, zOyColumsClose.at(i)->pos);
        modelMatrix = glm::scale(modelMatrix, zOyColumsClose.at(i)->size);

        glUniform3fv(shaders["globe"]->GetUniformLocation("columnCellColor"), 1, glm::value_ptr(zOyColumsClose.at(i)->correspondingCell->color));
        glUniform3fv(shaders["globe"]->GetUniformLocation("columnLightPosition"), 1, glm::value_ptr(zOyColumsClose.at(i)->correspondingCell->lightPos));

        DrawMeshWithTexture(modelMatrix, meshes["zOy_rect"]);
    }

    for (int i = 0; i < zOyColumsFar.size(); i++) {
        glm::mat4 modelMatrix(1.f);

        modelMatrix = glm::translate(modelMatrix, zOyColumsFar.at(i)->pos);
        modelMatrix = glm::scale(modelMatrix, zOyColumsFar.at(i)->size);

        glUniform3fv(shaders["globe"]->GetUniformLocation("columnCellColor"), 1, glm::value_ptr(zOyColumsFar.at(i)->correspondingCell->color));
        glUniform3fv(shaders["globe"]->GetUniformLocation("columnLightPosition"), 1, glm::value_ptr(zOyColumsFar.at(i)->correspondingCell->lightPos));

        DrawMeshWithTexture(modelMatrix, meshes["zOy_rect"]);
    }

    for (int i = 0; i < xOyColumsClose.size(); i++) {
        glm::mat4 modelMatrix(1.f);

        modelMatrix = glm::translate(modelMatrix, xOyColumsClose.at(i)->pos);
        modelMatrix = glm::scale(modelMatrix, xOyColumsClose.at(i)->size);

        glUniform3fv(shaders["globe"]->GetUniformLocation("columnCellColor"), 1, glm::value_ptr(xOyColumsClose.at(i)->correspondingCell->color));
        glUniform3fv(shaders["globe"]->GetUniformLocation("columnLightPosition"), 1, glm::value_ptr(xOyColumsClose.at(i)->correspondingCell->lightPos));

        DrawMeshWithTexture(modelMatrix, meshes["xOy_rect"]);
    }

    for (int i = 0; i < xOyColumsFar.size(); i++) {
        glm::mat4 modelMatrix(1.f);

        modelMatrix = glm::translate(modelMatrix, xOyColumsFar.at(i)->pos);
        modelMatrix = glm::scale(modelMatrix, xOyColumsFar.at(i)->size);

        glUniform3fv(shaders["globe"]->GetUniformLocation("columnCellColor"), 1, glm::value_ptr(xOyColumsFar.at(i)->correspondingCell->color));
        glUniform3fv(shaders["globe"]->GetUniformLocation("columnLightPosition"), 1, glm::value_ptr(xOyColumsFar.at(i)->correspondingCell->lightPos));

        DrawMeshWithTexture(modelMatrix, meshes["xOy_rect"]);
    }

    glUniform1i(shaders["globe"]->GetUniformLocation("isColumn"), 0);
}

void Tema3::DrawCeiling() {
    glm::mat4 modelMatrix(1.f);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, height, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(width, 0, length));

    DrawMeshWithTexture(modelMatrix, meshes["xOz_rect"]);
}

void Tema3::DrawGlobe() {
    glm::mat4 modelMatrix(1.f);

    modelMatrix = glm::translate(modelMatrix, globe->getPosition());
    modelMatrix = glm::scale(modelMatrix, globe->getScale());

    DrawMeshWithTexture(modelMatrix, meshes["globe"]);
}

void Tema3::DrawSpotLights() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaders["globe"]->Use();
    glUniform1i(shaders["globe"]->GetUniformLocation("isSpotlight"), 1);

    for (int i = 0; i < spotlights.size(); i++) {
        glm::mat4 modelMatrix(1.f);

        modelMatrix = glm::translate(modelMatrix, spotlights.at(i)->getPos());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(tan(RADIANS(spotlights.at(i)->getRadius())), 1, tan(RADIANS(spotlights.at(i)->getRadius()))) * height);

        glUniform3fv(shaders["globe"]->GetUniformLocation("spotlightColor"), 1, glm::value_ptr(spotlights.at(i)->getColor()));

        DrawMeshWithTexture(modelMatrix, meshes["cone"]);
    }

    glUniform1i(shaders["globe"]->GetUniformLocation("isSpotlight"), 0);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
}

void Tema3::DrawDancers() {
    shaders["globe"]->Use();
    glUniform1i(shaders["globe"]->GetUniformLocation("isDancer"), 1);

    for (auto dancer : dancers) {
        glm::mat4 modelMatrix(1.f);

        modelMatrix = glm::translate(modelMatrix, dancer->getPos() + dancer->getSize() / 2.f);
        modelMatrix = glm::scale(modelMatrix, dancer->getSize());

        glm::vec3 cellPositions[9];
        glm::vec3 cellColors[9];

        auto cells = dancer->getCorrespondingCells();
        for (int i = 0; i < cells.size(); i++) {
            cellPositions[i] = cells[i]->lightPos;
            cellColors[i] = cells[i]->color;
        }

        glUniform3fv(shaders["globe"]->GetUniformLocation("dancerCellPositions"), 9, glm::value_ptr(cellPositions[0]));
        glUniform3fv(shaders["globe"]->GetUniformLocation("dancerCellColors"), 9, glm::value_ptr(cellColors[0]));

        DrawMeshWithTexture(modelMatrix, meshes["dancer"]);
    }

    glUniform1i(shaders["globe"]->GetUniformLocation("isDancer"), 0);
}

void Tema3::UpdateDancers(float deltaTimeSeconds) {
    for (auto d : dancers) {
        d->move(deltaTimeSeconds, floor);
    }
}

void Tema3::DrawMeshWithTexture(glm::mat4& modelMatrix, Mesh* mesh) {
    shaders["globe"]->Use();

    glUniform1i(shaders["globe"]->GetUniformLocation("globeIsActive"), globe->isActive());
    glUniform1i(shaders["globe"]->GetUniformLocation("floorIsActive"), floorIsActive);
    glUniform1i(shaders["globe"]->GetUniformLocation("spotlightsAreActive"), spotlightsAreActive);

    glm::vec3 spotlightPositions[4];
    glm::vec3 spotlightColors[4];
    glm::vec3 spotlightDirections[4];
    for (int i = 0; i < spotlights.size(); i++) {
        spotlightPositions[i] = spotlights.at(i)->getPos();
        spotlightColors[i] = spotlights.at(i)->getColor();
        spotlightDirections[i] = spotlights.at(i)->getDirection();
    }

    glUniform3fv(shaders["globe"]->GetUniformLocation("spotlightPositions"), 4, glm::value_ptr(spotlightPositions[0]));
    glUniform3fv(shaders["globe"]->GetUniformLocation("spotlightColors"), 4, glm::value_ptr(spotlightColors[0]));
    glUniform3fv(shaders["globe"]->GetUniformLocation("spotlightDirections"), 4, glm::value_ptr(spotlightDirections[0]));

    glUniformMatrix4fv(shaders["globe"]->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(shaders["globe"]->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
    glUniformMatrix4fv(shaders["globe"]->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));

    glUniform3fv(shaders["globe"]->GetUniformLocation("globe_position"), 1, glm::value_ptr(globe->getPosition()));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, globe->getTexture()->GetTextureID());
    glUniform1i(shaders["globe"]->loc_textures[0], 0);

    glUniform1f(shaders["globe"]->GetUniformLocation("time"), glfwGetTime());
    glUniform3fv(shaders["globe"]->loc_eye_pos, 1, glm::value_ptr(GetSceneCamera()->m_transform->GetWorldPosition()));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Tema3::OnKeyPress(int key, int mods) {
    if (window->KeyHold(GLFW_KEY_Z)) {
        globe->changeState();
    }

    if (window->KeyHold(GLFW_KEY_X)) {
        floorIsActive = !floorIsActive;
    }

    if (window->KeyHold(GLFW_KEY_V)) {
        spotlightsAreActive = !spotlightsAreActive;
    }
}