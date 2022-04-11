#pragma once

#include "lab_m1/tema 2/Player/Hitbox.h"

class EnemyTema2 {
	private:
		Hitbox* hitbox;

		float speed;
		float theta;
		float r;

		glm::vec3 center;

		float deathAnimationTime;
		float currentTime;

		int deathStage;

	public:
		EnemyTema2(glm::vec3 pos, glm::vec3 dimensions);

		Hitbox* getHitbox();
		int getDeathStage();

		void Move(float deltaTimeSeconds);

		void startDeathAnimation();
};