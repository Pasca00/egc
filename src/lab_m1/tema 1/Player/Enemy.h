#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema 1/Player/Movable.h"
#include "lab_m1/tema 1/Map/WorldObject.h"

class Enemy : public Movable {
	private:
		Movable* player;

	public:
		float offset;

		Enemy(Movable* player, float width, float height);
		~Enemy();

		void moveToPlayer(float deltaTimeSeconds);

		void handleCollision(Movable* m) override;

		void computeRotationAngle();

		static void createMesh(std::unordered_map<std::string, Mesh*>& meshes);
};