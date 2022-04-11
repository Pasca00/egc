#pragma once

#include <time.h>

#include "components/simple_scene.h"

#include "Player/Entity.h"
#include "Player/Enemy.h"
#include "lab_m1/tema 1/Map/Map.h"
#include "Player/Projectile.h"

namespace m1 {
	class Tema1 : public gfxc::SimpleScene {
		public:
			struct ViewportSpace {
				ViewportSpace() : x(0), y(0), width(1), height(1) {}
				ViewportSpace(int x, int y, int width, int height)
					: x(x), y(y), width(width), height(height) {}
				int x;
				int y;
				int width;
				int height;
			};

			Tema1();
			~Tema1();

			void Init() override;
			void FrameStart() override;
			void Update(float deltaTimeSeconds) override;
			void OnInputUpdate(float deltaTimeSeconds, int mods) override;

			void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear);
			glm::mat3 Tema1::VisualizationTransf2DUnif(const Rect& logicSpace, const ViewportSpace& viewSpace);

		private:
			Entity* player;
			ViewportSpace viewSpace;
			Rect logicSpace;

			WorldMap* map;

			std::vector<Enemy*> enemies;
			std::vector<Projectile*> projectiles;

			float viewSpaceX;
			float viewSpaceY;

			float enemySpawnRate;
			float enemySpawnAccum;

			void spawnEnemy(float deltaTimeSeconds);

			void moveProjectiles(float deltaTimeSeconds);
			void moveEnemies(float deltaTimeSeconds);

			void RenderEnemies(glm::mat3& visMatrix);
			void RenderProjectiles(glm::mat3& visMatrix);
			void RenderObjects(glm::mat3& visMatrix);
			void RenderHealthbar();

			void checkCollisions();
			void checkPlayerWorldCollisions();
			void checkPlayerEnemyCollisions();
			void checkProjectilesEnemyCollisions();
			void checkProjectilesWorldCollisions();
	};
}