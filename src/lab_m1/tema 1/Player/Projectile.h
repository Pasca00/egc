#pragma once

#include "lab_m1/tema 1/Player/Movable.h"

class Projectile : public Movable {
	private:
		int* playerScore;

	public:
		Projectile(Rect& playerPos, int* playerScore, float rotationAngle);

		void move(float deltaTimeSeconds);
		void handleCollision(Movable* m) override;
};