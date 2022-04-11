#include "lab_m1/tema 1/Player/Enemy.h"

Enemy::Enemy(Movable* player, float width, float height) : Movable(200, 300, 100, 100), player(player), offset(50) {
	moveSpeed = 180 + rand() % 220;
	bounds.x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / width));
	bounds.y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / height));

	rotationAngle = 0;

	dmg = 10 + rand() % 11;
}

Enemy::~Enemy() { }

void Enemy::createMesh(std::unordered_map<std::string, Mesh*>& meshes) {
	glm::vec3 color(0.085f, 0.28f, 0.66f);
	glm::vec3 colorHands(0.2f, 0.09f, 0.27f);

	std::vector<VertexFormat> vertices{
		// Body
		VertexFormat(glm::vec3(-50, -50, 0), color, glm::vec3(0)),
		VertexFormat(glm::vec3(50, -50, 0), color, glm::vec3(0)),
		VertexFormat(glm::vec3(-50, 50, 0), color, glm::vec3(0)),
		VertexFormat(glm::vec3(50, 50, 0), color, glm::vec3(0)),
		// Left hand
		VertexFormat(glm::vec3(-40, 40, 0), colorHands, glm::vec3(0)),
		VertexFormat(glm::vec3(-10, 40, 0), colorHands, glm::vec3(0)),
		VertexFormat(glm::vec3(-40, 70, 0), colorHands, glm::vec3(0)),
		VertexFormat(glm::vec3(-10, 70, 0), colorHands, glm::vec3(0)),
		// Right hand
		VertexFormat(glm::vec3(10, 40, 0), colorHands, glm::vec3(0)),
		VertexFormat(glm::vec3(40, 40, 0), colorHands, glm::vec3(0)),
		VertexFormat(glm::vec3(10, 70, 0), colorHands, glm::vec3(0)),
		VertexFormat(glm::vec3(40, 70, 0), colorHands, glm::vec3(0))
	};

	std::vector<unsigned int> indeces {
		// Body
		0, 1, 2,
		1, 3, 2,
		// Left hand
		4, 5, 6,
		5, 7, 6,
		// Right hand
		8, 9, 10,
		9, 11, 10
	};

	Mesh* enemy = new Mesh("enemy");
	enemy->SetDrawMode(GL_TRIANGLES);
	enemy->InitFromData(vertices, indeces);
	meshes["enemy"] = enemy;
}

void Enemy::moveToPlayer(float deltaTimeSeconds) {
	if (deltaTimeSeconds != INFINITY) {
		Rect playerPos = player->getBounds();
		float angle = atan2(playerPos.y - bounds.y, playerPos.x - bounds.x);

		bounds.x += deltaTimeSeconds * cos(angle) * moveSpeed;
		bounds.y += deltaTimeSeconds * sin(angle) * moveSpeed;
	}

}

void Enemy::handleCollision(Movable* m) { delete this; }

void Enemy::computeRotationAngle() {
	Rect playerPos = player->getBounds();
	rotationAngle = -atan2(playerPos.x - bounds.x, playerPos.y - bounds.y);
}