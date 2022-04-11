#include "lab_m1/tema 1/Map/WorldObject.h"

WorldObject::WorldObject(float x, float y, float w, float h, glm::vec3 color)
	: bounds(x, y, w, h), color(color), passable(false) { }

void WorldObject::becomePassable() {
	passable = true;
}

