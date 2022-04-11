#pragma once

#include "lab_m1/tema 2/Player/Hitbox.h"

class Tile {
	public:
		Tile(int value, glm::vec3 pos, glm::vec3 dimensions);
		~Tile();

		int value;
		Hitbox* hitbox;

		void setValue(int value);
		void setPos(glm::vec3 pos);
};