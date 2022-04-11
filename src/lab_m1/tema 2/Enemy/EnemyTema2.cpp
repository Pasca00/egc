#include "lab_m1/tema 2/Enemy/EnemyTema2.h"

EnemyTema2::EnemyTema2(glm::vec3 pos, glm::vec3 dimensions) {
	hitbox = new Hitbox(pos, dimensions);

	center = pos;
	theta = 0;
	r = 0.6f;

	speed = 2.f;

	deathStage = 0;
	deathAnimationTime = 5;
	currentTime = 0;
}

Hitbox* EnemyTema2::getHitbox() {
	return hitbox;
}

int EnemyTema2::getDeathStage() {
	return deathStage;
}

void EnemyTema2::Move(float deltaTimeSeconds) {
	hitbox->pos.x = r * cos(theta) + center.x;
	hitbox->pos.z = r * sin(theta) + center.z;

	theta += 9.5f * deltaTimeSeconds;

	if (deathStage == 1) {
		currentTime += deltaTimeSeconds;
		if (currentTime >= deathAnimationTime) {
			deathStage = 2;
		}
	}
}

void EnemyTema2::startDeathAnimation() {
	deathStage = 1;
}