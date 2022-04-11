#pragma once

#include "glm/glm.hpp"

struct Rect {
	Rect(float x, float y, float w, float h) 
		: x(x), y(y), w(w), h(h) { }

	float x;
	float y;
	float w;
	float h;
};

class WorldObject {
	private:
		bool passable;

	public:
		Rect bounds;
		glm::vec3 color;

		WorldObject(float x, float y, float w, float h, glm::vec3 color);
		void becomePassable();
};