#include "lab_m1/tema 1/Player/Projectile.h"
#include "utils/math_utils.h"

Projectile::Projectile(Rect& playerPos, int* playerScore, float rotationAngle) 
	: Movable(playerPos.x, playerPos.y, 10, 10) {
	this->rotationAngle = rotationAngle + M_PI_2;
	this->playerScore = playerScore;

	moveSpeed = 600;
}

void Projectile::move(float deltaTimeSeconds) {
	bounds.x += deltaTimeSeconds * moveSpeed * cos(rotationAngle);
	bounds.y += deltaTimeSeconds * moveSpeed * sin(rotationAngle);
}

void Projectile::handleCollision(Movable* m) {
	m->handleCollision(NULL);
	(*playerScore)++;
	system("cls");
	printf("Your score is now: %d\n", *playerScore);
	delete this;
}