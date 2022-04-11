#include "lab_m1/tema 3/Dancer.h"

std::vector<Movement> Dancer::movementOptions = {CIRCULAR, DIAGONAL, VERTICAL, HORIZONTAL};

Dancer::Dancer(Movement movement, glm::vec3 pos, glm::vec3 size) {
	this->pos = pos;
	this->originalPos = pos;
	this->size = size;

	this->speed = (rand() % 9 + 2) / 10.f;

	this->t = 0;
	
	this->direction = rand() % 2;
	if (direction == 0.f) {
		direction = -1.f;
	}

	this->movement = movement;
}

void Dancer::move(float deltaTimeSeconds, std::vector<std::vector<LightCell*>>& floor) {
	switch (movement) {
	case CIRCULAR:
		pos = originalPos + glm::vec3(2.f * cos(t), 0, 2.f * sin(t));
		t += direction * speed * deltaTimeSeconds;

		break;

	case DIAGONAL:
		pos += direction * glm::vec3(speed * deltaTimeSeconds, 0, speed * deltaTimeSeconds);
		if (glm::distance(pos, originalPos) > 2.0) {
			if (direction == 1.f) {
				direction = -1.f;
			} else {
				direction = 1.f;
			}
		}

		break;

	case VERTICAL:
		pos += direction * glm::vec3(speed * deltaTimeSeconds, 0, 0);
		if (glm::distance(pos, originalPos) > 2.0) {
			if (direction == 1.f) {
				direction = -1.f;
			}
			else {
				direction = 1.f;
			}
		}

		break;

	case HORIZONTAL:
		pos += direction * glm::vec3(0, 0, speed * deltaTimeSeconds);
		if (glm::distance(pos, originalPos) > 2.0) {
			if (direction == 1.f) {
				direction = -1.f;
			}
			else {
				direction = 1.f;
			}
		}

		break;
	}

	findCorrespondingCells(floor);
}

glm::vec3 Dancer::getPos() {
	return pos;
}

glm::vec3 Dancer::getSize() {
	return size;
}

std::vector<LightCell*> Dancer::getCorrespondingCells() {
	return correspondingCells;
}

void Dancer::findCorrespondingCells(std::vector<std::vector<LightCell*>>& floor) {
	correspondingCells.clear();
	for (int i = 0; i < floor.size(); i++) {
		for (int j = 0; j < floor[i].size(); j++) {
			if (pos.x >= floor[i][j]->pos.x && pos.x <= floor[i][j]->pos.x + floor[i][j]->size.x
				&& pos.z >= floor[i][j]->pos.z && pos.z <= floor[i][j]->pos.z + floor[i][j]->size.z) {

				for (int ki = i - 1; ki < i - 1 + 3 && ki >= 0 && ki < floor.size(); ki++) {
					for (int kj = j - 1; kj < j - 1 + 3 && kj >= 0 && kj < floor[ki].size(); kj++) {
						correspondingCells.push_back(floor[i][j]);
					}
				}

				return;
			}
		}
	}
}