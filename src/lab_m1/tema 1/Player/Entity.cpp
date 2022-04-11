#include "lab_m1/tema 1/Player/Entity.h"
#include "lab_m1/tema 1/Map/Map.h"

Entity::Entity(std::unordered_map<std::string, Mesh*>& meshes)
	: Movable(300, 800, 50, 50) {
	meshes["playerMesh"] = createPlayerMesh();

	health = 100;
	maxHealth = 100;

	runningMultiplier = 1;
	gunFireAccum = 0;
	gunFireRate = 1;

	score = 0;

	healthbar = new HealthBar(meshes);
}

Entity::~Entity() { }

Mesh* Entity::createPlayerMesh() {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	float radius = 20;

	for (int i = 0; i < 360; i++) {
		vertices.push_back(VertexFormat(
			glm::vec3(35 + radius * cos(i), 35 + radius * sin(i), 0),
			glm::vec3(0.55f, 0.089f, 0.15f),
			glm::vec3(0, 0, 0)
		));
		indices.push_back(i);
	}

	for (int i = 0; i < 360; i++) {
		vertices.push_back(VertexFormat(
			glm::vec3(-35 + radius * cos(i), 35 + radius * sin(i), 0),
			glm::vec3(0.55f, 0.089f, 0.15f),
			glm::vec3(0, 0, 0)
		));
		indices.push_back(360 + i);
	}

	radius = 50;
	for (int i = 0; i < 360; i++) {
		vertices.push_back(VertexFormat(
			glm::vec3(radius * cos(i), radius * sin(i), 0),
			glm::vec3(0.91f, 0.09f, 0.13f),
			glm::vec3(0, 0, 0)
		));

		indices.push_back(720 + i);
	}

	const char* meshName = "playerMesh";

	Mesh* playerMesh = new Mesh(meshName);
	playerMesh->SetDrawMode(GL_TRIANGLES);
	playerMesh->InitFromData(vertices, indices);
	

	return playerMesh;
}

void Entity::handleInput(WindowObject* window, Rect& logicSpace, std::vector<Projectile*>& projectiles, float deltaTimeSeconds, int mods) {
	if (window->KeyHold(GLFW_KEY_LEFT_SHIFT)) {
		runningMultiplier = 2;
	} else {
		runningMultiplier = 1;
	}

	if (window->KeyHold(GLFW_KEY_W) && !this->collidesUp()) {
		yDirection = Y_DIR_UP;
		bounds.y += deltaTimeSeconds * moveSpeed * runningMultiplier;
		logicSpace.y += deltaTimeSeconds * moveSpeed * runningMultiplier;
	} 
	if (window->KeyHold(GLFW_KEY_S) && !this->collidesDown()) {
		yDirection = Y_DIR_DOWN;
		bounds.y -= deltaTimeSeconds * moveSpeed * runningMultiplier;
		logicSpace.y -= deltaTimeSeconds * moveSpeed * runningMultiplier;
	}
	if (window->KeyHold(GLFW_KEY_A) && !this->collidesLeft()) {
		xDirection = X_DIR_LEFT;
		bounds.x -= deltaTimeSeconds * moveSpeed * runningMultiplier;
		logicSpace.x -= deltaTimeSeconds * moveSpeed * runningMultiplier;
	}
	if (window->KeyHold(GLFW_KEY_D) && !this->collidesRight()) {
		xDirection = X_DIR_RIGHT;
		bounds.x += deltaTimeSeconds * moveSpeed * runningMultiplier;
		logicSpace.x += deltaTimeSeconds * moveSpeed * runningMultiplier;
	}

	computeRotationAngle(window->GetCursorPosition(), window->GetResolution());

	gunFireAccum += deltaTimeSeconds;
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		if (gunFireAccum >= gunFireRate) {
			gunFireAccum = 0;
			projectiles.push_back(new Projectile(bounds, &score, rotationAngle));
		}
	}
}

void Entity::computeRotationAngle(glm::vec2 cursorPos, glm::vec2 windowRes) {
	float theta = atan2(cursorPos.x - (windowRes.x / 2), (windowRes.y / 2) - cursorPos.y);
	rotationAngle = -theta;
}

float Entity::getMoveSpeed() {
	return moveSpeed * runningMultiplier;
}

int Entity::getScore() {
	return score;
}

void Entity::handleCollision(Movable* e) {
	if (health - e->dmg > 0) { 
		health -= e->dmg;
	}
	else {
		health = 0;
	}

	float percent = (float)health / maxHealth;
	healthbar->currentColor = glm::vec3(1.f - (1.f * percent), 1.f * percent, 0);
	healthbar->bounds.w = 200 * percent;
}

Entity::HealthBar::HealthBar(std::unordered_map<std::string, Mesh*>& meshes) : bounds(40, 600, 200, 50) {

	currentColor = glm::vec3(0, 1.f, 0);

	glm::vec3 color(0, 0.8f, 0.2f);
	std::vector<VertexFormat> vertices {
		VertexFormat(glm::vec3(40, 600, 0), color, glm::vec3(0)),
		VertexFormat(glm::vec3(240, 600, 0), color, glm::vec3(0)),
		VertexFormat(glm::vec3(40, 650, 0), color, glm::vec3(0)),
		VertexFormat(glm::vec3(240, 650, 0), color, glm::vec3(0))
	};

	std::vector<unsigned int> indices {
		1, 3, 2, 0,
		0, 1, 3, 2
	};

	Mesh* healthbarContainer = new Mesh("healthbarContainer");
	healthbarContainer->SetDrawMode(GL_LINES);
	healthbarContainer->InitFromData(vertices, indices);
	meshes[healthbarContainer->GetMeshID()] = healthbarContainer;
}

bool Entity::isDead() {
	return health == 0;
}