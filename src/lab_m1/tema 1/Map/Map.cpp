#include "lab_m1/tema 1/Map/Map.h"

WorldMap::WorldMap(float w, float h, std::unordered_map<std::string, Mesh*>& meshes) {
	bounds = { 0, 0, w, h };

	glm::vec3 mapColor(.89f, .68f, .27f);

	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(0, 0, 0), mapColor, glm::vec3(0)),
		VertexFormat(glm::vec3(w, 0, 0), mapColor, glm::vec3(0)),
		VertexFormat(glm::vec3(0, h, 0), mapColor, glm::vec3(0)),
		VertexFormat(glm::vec3(w, h, 0), mapColor, glm::vec3(0))
	};

	std::vector<unsigned int> indices = {
		0, 1, 2,
		1, 3, 2
	};

	Mesh* worldMap = new Mesh("worldMap");
	worldMap->SetDrawMode(GL_TRIANGLES);
	worldMap->InitFromData(vertices, indices);
	meshes["worldMap"] = worldMap;

	std::vector<VertexFormat> v = {
		VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(0)),
		VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0), glm::vec3(0)),
		VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(0)),
		VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0), glm::vec3(0))
	};

	std::vector<unsigned int> i = {
		0, 1, 2,
		1, 3, 2
	};
	
	Mesh* rect = new Mesh("rect");
	rect->SetDrawMode(GL_TRIANGLES);
	rect->InitFromData(v, i);
	meshes["rect"] = rect;

	createObjects();
}

void WorldMap::createObjects() {
	glm::vec3 wallColor(0.47f, 0.39f, 0.23f);
	objects = std::vector<WorldObject*>{
		new WorldObject(0, bounds.height - 200, 900, 200, wallColor),
		new WorldObject(0, 0, 800, 200, wallColor),
		new WorldObject(0, 0, 200, 600, wallColor),
		new WorldObject(1000, 0, 800, 200, wallColor),
		new WorldObject(0, 750, 200, 400, wallColor),
		new WorldObject(800, 350, 250, 250, wallColor),
		new WorldObject(bounds.width - 500, bounds.height - 200, 500, 200, wallColor),
		new WorldObject(1600, 450, 250, 250, wallColor),
		new WorldObject(1200, 700, 250, 250, wallColor)
	};
}

std::vector<WorldObject*> WorldMap::getObjects() {
	return objects;
}

WorldMap::~WorldMap() { }