#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"

#include "lab_m1/tema 2/Camera/MyCamera.h"
#include "lab_m1/tema 2/Player/Player.h"
#include "lab_m1/tema 2/Enemy/EnemyTema2.h"
#include "lab_m1/tema 2/Map/TileMap.h"
#include "lab_m1/tema 2/Player/Bullet.h"

namespace m1 {
	class Tema2 : public gfxc::SimpleScene {
	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void OnInputUpdate(float deltaTimeSeconds, int mods) override;

		void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;

		MyCamera* camera;

		TileMap* tileMap;
		std::unordered_map<std::string, Texture2D*> mapTextures;

		Player* player;
		std::vector<Bullet*> bullets;

		std::vector<EnemyTema2*> enemies;

		void createBasicRectMesh();
		void RenderMapBounds();
		void RenderWalls();

		void RenderPlayer();

		void MoveBullets(float deltaTimeSeconds);
		void RenderBullets();


		void CheckBulletEnemyCollisions();
		void CheckBulletWorldCollisons();
		void CheckPlayerEnemyCollision();
		void CheckIfPlayerEscaped();

		void RenderMeshWithTexture(Mesh* mesh, Texture2D* texture, glm::mat4& modelMatrix);

		void PlaceEnemies();
		void RenderEnemies(float deltaTimeSeconds);
		void MoveEnemies(float deltaTimeSeconds);

		void RenderContainers();

	public:
		Tema2();
		~Tema2();
		void Init() override;

	};
}