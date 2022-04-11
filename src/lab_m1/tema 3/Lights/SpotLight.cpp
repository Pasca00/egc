#include "lab_m1/tema 3/Lights/SpotLight.h"

Spotlight::Spotlight(glm::vec3 pos) {
	this->pos = pos;
	this->direction = glm::vec3(0, -1, 0);

	this->color = glm::vec3(rand() % 256 / 256.f, rand() % 256 / 256.f, rand() % 256 / 256.f);
	this->radius = 12.f;
	this->spotAngle = 0.5f;
}

glm::vec3 Spotlight::getPos() {
	return pos;
}

glm::vec3 Spotlight::getColor() {
	return color;
}

glm::vec3 Spotlight::getDirection() {
	return direction;
}

float Spotlight::getAngle() {
	return spotAngle;
}

float Spotlight::getRadius() {
	return radius;
}