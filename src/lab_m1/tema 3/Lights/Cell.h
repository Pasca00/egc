#pragma once

#include "glm/glm.hpp"

class LightCell {
	public:
		glm::vec3 color;
		glm::vec3 pos;
		glm::vec3 size;
		glm::vec3 lightPos;

		LightCell(int i, int j, int y);
};