#pragma once;

#include "glm/glm.hpp"

class Spotlight {
	private:
		glm::vec3 pos;
		glm::vec3 direction;
		glm::vec3 color;

		float spotAngle;
		float radius;

	public:
		Spotlight(glm::vec3 pos);

		glm::vec3 getPos();
		glm::vec3 getColor();
		glm::vec3 getDirection();
		
		float getAngle();
		float getRadius();
};