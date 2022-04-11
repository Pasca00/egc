#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "lab_m1/tema 3/Lights/Cell.h"

enum Movement {
	CIRCULAR, DIAGONAL, VERTICAL, HORIZONTAL
};

class Dancer {
	private:
		glm::vec3 pos;
		glm::vec3 originalPos;
		glm::vec3 size;

		float speed;
		float direction;

		float t;

		std::vector<LightCell*> correspondingCells;

		 Movement movement;

	public:
		Dancer(Movement movement, glm::vec3 pos = glm::vec3(0), glm::vec3 size = glm::vec3(1.f, 2.5f, 1.f));

		void move(float deltaTimeSeconds, std::vector<std::vector<LightCell*>>& floor);

		glm::vec3 getPos();
		glm::vec3 getSize();

		std::vector<LightCell*> getCorrespondingCells();

		void findCorrespondingCells(std::vector<std::vector<LightCell*>>& floor);

		static std::vector<Movement> movementOptions;
};