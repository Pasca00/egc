#pragma once

#include "glm/glm.hpp"
#include "lab_m1/tema 3/Lights/Cell.h"

struct Column {
	Column(glm::vec3 pos, glm::vec3 size, LightCell* correspondingCell);

	glm::vec3 pos;
	glm::vec3 size;
	
	LightCell* correspondingCell;
};