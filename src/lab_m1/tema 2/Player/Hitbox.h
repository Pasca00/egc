#pragma once

#include "utils/glm_utils.h"

struct Hitbox {
	glm::vec3 pos;
	glm::vec3 dimensions;

	Hitbox(glm::vec3 pos, glm::vec3 dimensions) : pos(pos), dimensions(dimensions) { }
};