#include "Tile.h"

Tile::Tile(int value, glm::vec3 pos, glm::vec3 dimensions) {
	this->value = value;
	this->hitbox = new Hitbox(pos, dimensions);
}

Tile::~Tile() {
	delete hitbox;
}

void Tile::setValue(int value) {
	this->value = value;
}

void Tile::setPos(glm::vec3 pos) {
	hitbox->pos = pos;
}