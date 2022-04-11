#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema 1/Map/WorldObject.h"

class WorldMap {
	public:
		struct LogicSpace {
			LogicSpace() : x(0), y(0), width(1), height(1) {}
			LogicSpace(float x, float y, float width, float height)
				: x(x), y(y), width(width), height(height) {}
			float x;
			float y;
			float width;
			float height;
		};

		WorldMap(float w, float h, std::unordered_map<std::string, Mesh*>& meshes);
		~WorldMap();

		std::vector<WorldObject*> getObjects();

		LogicSpace bounds;

	private:
		std::vector<WorldObject*> objects;

		void createObjects();
};