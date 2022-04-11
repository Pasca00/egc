#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "lab_m1/tema 2/Map/Tile.h"

class TileMap {
	private:
		int w;
		int l;
		std::vector<std::vector<Tile*>> layout;

		glm::vec3 tileColor;

		float worldW;
		float worldL;

		void createLayout();

	public:
		TileMap(int w, int l);

		std::vector<std::vector<Tile*>> getLayout();

		float getWorldWidth();
		float getWorldLength();

		float tileW;
		float tileL;

		std::pair<float, float> startPos;
		std::pair<int, int> endPos;
};