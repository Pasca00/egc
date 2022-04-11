#include "lab_m1/tema 2/Player/ResourceContainer.h"

ResourceContainer::ResourceContainer(float maxValue) {
	this->hitbox = new Hitbox(glm::vec3(1.f, 1.f, 0.f), glm::vec3(3.f, 0.5f, 0.f));

	this->maxWidth = 3.f;

	this->maxValue = maxValue;
	this->currentValue = maxValue;

	this->color = glm::vec3(1);
}

void ResourceContainer::updateValue(float value) {
	currentValue += value;

	currentValue = MAX(0.f, currentValue);

	float p = currentValue / maxValue;
	hitbox->dimensions.x = maxWidth * p;
}

Hitbox* ResourceContainer::getHitbox() {
	return hitbox;
}

ResourceContainer* ResourceContainer::setPos(glm::vec3 pos) {
	hitbox->pos = pos;
	return this;
}

ResourceContainer* ResourceContainer::setColor(glm::vec3 color) {
	this->color = color;
	return this;
}

glm::vec3 ResourceContainer::getColor() {
	return color;
}

float ResourceContainer::getMaxWidth() {
	return maxWidth;
}

bool ResourceContainer::isEmpty() {
	return currentValue == 0;
}