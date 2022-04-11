#include "lab_m1/tema 2/Player/Bullet.h"


Bullet::Bullet(glm::vec3 startPos, glm::vec3 forward) {
	speed = 30.f;
	ttl = 5;
	currentTime = 0;

	direction = forward;

	hitbox = new Hitbox(startPos, glm::vec3(0.2f, 0.1f, 0.1f));
}

Bullet::~Bullet() {
	delete hitbox;
}

Hitbox* Bullet::getHitbox() {
	return hitbox;
}

glm::vec3 Bullet::getPos() {
	return hitbox->pos;
}

glm::vec3 Bullet::getDimensions() {
	return hitbox->dimensions;
}

bool Bullet::hasExpired() {
	return currentTime >= ttl;
}

void Bullet::move(float deltaTimeSeconds) {
	hitbox->pos += direction * speed * deltaTimeSeconds;
	currentTime += deltaTimeSeconds;
}