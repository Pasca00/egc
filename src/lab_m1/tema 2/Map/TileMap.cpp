#include "lab_m1/tema 2/Map/TileMap.h"

#include <stack>

void printLayout(std::vector<std::vector<Tile*>>& layout) {
	for (int i = 0; i < layout.size(); i++) {
		for (int j = 0; j < layout[i].size(); j++) {
			printf("%d ", layout[i][j]->value);
		}
		printf("\n");
	}
}

TileMap::TileMap(int w, int l) {
	this->w = w;
	this->l = l;


	this->tileW = 5.f;
	this->tileL = 5.f;
	this->tileColor = glm::vec3(0);

	this->worldW = tileW * w;
	this->worldL = tileL * l;

	this->layout = std::vector<std::vector<Tile*>>(w, std::vector<Tile*>(l, NULL));
	
	createLayout();
}

void TileMap::createLayout() {
	for (int i = 0; i < layout.size(); i++) {
		for (int j = 0; j < layout[i].size(); j++) {
			layout[i][j] = new Tile(1, glm::vec3(i * tileW, 0, j * tileL), glm::vec3(tileW, 10, tileL));
		}
	}

	// Choose starting point
	int startX = rand() % (w - 1) + 1;
	int startY = rand() % (l - 1) + 1;

	if (startX % 2 == 0) {
		if (startX + 1 == w) {
			startX--;
		} else {
			startX++;
		}
	}

	if (startY % 2 == 0) {
		if (startY + 1 == l) {
			startY--;
		} else {
			startY++;
		}
	}

	startPos = { startX * tileW + tileW / 2, startY * tileL + tileL / 2};

	std::stack<std::pair<int, int>> s;
	s.push({startX, startY});

	layout[startX][startY]->value = 0;
	
	bool exitPlaced = false;
	int enemyGenProb = 12;
	int requiredDistance = 15;
	int currentDistance = 0;

	while (!s.empty()) {
		std::pair<int, int> curr = s.top();

		// Build list of unvisited neighbours
		std::vector<std::pair<int, int>> ns;
		if (curr.first - 2 >= 0 && layout[curr.first - 2][curr.second]->value == 1) {
			ns.push_back({curr.first - 2, curr.second});
		}
		if (curr.first + 2 < layout.size() && layout[curr.first + 2][curr.second]->value == 1) {
			ns.push_back({curr.first + 2, curr.second});
		}
		if (curr.second - 2 >= 0 && layout[curr.first][curr.second - 2]->value == 1) {
			ns.push_back({curr.first, curr.second - 2});
		}
		if (curr.second + 2 < layout[curr.first].size() && layout[curr.first][curr.second + 2]->value == 1) {
			ns.push_back({curr.first, curr.second + 2});
		}

		if (ns.empty()) {
			s.pop();
		} else {
			int i = rand() % ns.size();
			int newX = ns[i].first;
			int newY = ns[i].second;

			int placeEnemy = rand() % enemyGenProb;
			if (placeEnemy == 1) {
				enemyGenProb = 12;
				placeEnemy = 2;
			} else {
				enemyGenProb = MAX(enemyGenProb - 1, 1);
				placeEnemy = 0;
			}

			layout[newX][newY]->setValue(placeEnemy);
			layout[(newX + curr.first) / 2][(newY + curr.second) / 2]->setValue(0);

			s.push({newX, newY});

			currentDistance++;

			if (currentDistance > requiredDistance && !exitPlaced) {
				if (curr.first - 1 == 0) {
					layout[curr.first - 1][curr.second]->setValue(4);
					exitPlaced = true;
					endPos = { curr.first - 1, curr.second };
				} else if (curr.first + 1 == w - 1) {
					layout[curr.first + 1][curr.second]->setValue(4);
					exitPlaced = true;
					endPos = { curr.first + 1, curr.second };
				} else if (curr.second - 1 == 0) {
					layout[curr.first][curr.second - 1]->setValue(4);
					exitPlaced = true;
					endPos = { curr.first, curr.second - 1 };
				} else if (curr.second + 1 == l - 1) {
					layout[curr.first][curr.second + 1]->setValue(4);
					exitPlaced = true;
					endPos = { curr.first, curr.second + 1 };
				}
			}
		}
	}
}

std::vector<std::vector<Tile*>> TileMap::getLayout() {
	return layout;
}


float TileMap::getWorldLength() {
	return worldL;
}

float TileMap::getWorldWidth() {
	return worldL;
}
