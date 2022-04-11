#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema 1/Player/Movable.h"
#include "lab_m1/tema 1/Player/Projectile.h"
#include "lab_m1/tema 1/Map/WorldObject.h"

class Entity : public Movable {
	private:
		std::string meshName;

		int health;
		int maxHealth;
		int score;

		float gunFireRate;
		float gunFireAccum;

		float speed;
		float runningMultiplier;

	public:

		Entity(std::unordered_map<std::string, Mesh*>& meshes);
		~Entity();

		Mesh* createPlayerMesh();
		void handleInput(WindowObject* window, Rect &logicSpace, std::vector<Projectile*>& projectiles, float deltaTimeSeconds, int mods);

		void computeRotationAngle(glm::vec2 cursorPos, glm::vec2 windowRes);

		float getMoveSpeed() override;
		int getScore();

		void handleCollision(Movable* m) override;

		class HealthBar {
			public:
				glm::vec3 currentColor;
				Rect bounds;

				HealthBar(std::unordered_map<std::string, Mesh*>& meshes);

		} *healthbar;

		bool isDead();
};