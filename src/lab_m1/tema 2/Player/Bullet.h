#pragma once

#include "lab_m1/tema 2/Player/Hitbox.h"

class Bullet {
	private:
		float speed;
		float ttl;
		float currentTime;

		glm::vec3 direction;

		Hitbox* hitbox;

	public:
		Bullet(glm::vec3 startPos, glm::vec3 forward);
		~Bullet();

		Hitbox* getHitbox();
		glm::vec3 getPos();
		glm::vec3 getDimensions();

		bool hasExpired();

		void move(float deltaTimeSeconds);
};