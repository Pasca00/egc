#pragma once

#include "glm/glm.hpp"
#include "core/gpu/texture2D.h"

class Globe {
	private:
		glm::vec3 position;
		glm::vec3 scale;

		int resolution;
		unsigned char* data;

		Texture2D* texture;

		bool active;

	public:
		Globe(glm::vec3 position);

		void changeState();

		glm::vec3 getPosition();
		glm::vec3 getScale();
		unsigned char* getData();

		bool isActive();

		Texture2D* getTexture();
};