#pragma once

#include "glm/glm.hpp"
#include "Hitbox.h"

class ResourceContainer {
	private:
		float maxValue;
		float currentValue;

		float maxWidth;

		Hitbox* hitbox;
		glm::vec3 color;

	public:
		ResourceContainer(float maxValue);

		void updateValue(float value);

		Hitbox* getHitbox();
		glm::vec3 getColor();
		float getMaxWidth();
		
		ResourceContainer* setPos(glm::vec3 pos);
		ResourceContainer* setColor(glm::vec3 color);

		bool isEmpty();
};