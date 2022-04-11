#include "lab_m1/tema 3/Lights/Cell.h"

LightCell::LightCell(int i, int j, int y) {
	color = glm::vec3(rand() % 256 / 256.f, rand() % 256 / 256.f, rand() % 256 / 256.f);

	size = glm::vec3(2.5f, 6.f, 2.5f);

	pos = glm::vec3(i * size.x, y * size.y, j * size.z);

	lightPos = glm::vec3(pos.x + size.x / 2, y * size.y, pos.z + size.z / 2);
}