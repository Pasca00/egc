#include "lab_m1/tema 2/Player/Player.h"

Player::Player(float x, float y, unordered_map<string, Mesh*>& meshes, unordered_map<string, Texture2D*>& textures, string& meshPath, string& texturePath) {
	hitbox = new Hitbox(glm::vec3(x, 3, y), glm::vec3(2.f));
	moveSpeed = 6.0f;

	createMesh(meshes, textures, meshPath, texturePath);

	fireRate = 2;
	timeSinceLastBullet = 2;

	health = new ResourceContainer(100);
	escapeTimer = new ResourceContainer(60);

	health->setColor(glm::vec3(0.f, 0.66f, 0.f))->setPos(glm::vec3(0.5f, 6.f, 0.f));
	escapeTimer->setColor(glm::vec3(0.75f))->setPos(glm::vec3(0.5f, 5.f, 0.f));

	collidesFront = false;
	collidesBack  = false;
	collidesLeft  = false;
	collidesRight = false;
}

void Player::createMesh(std::unordered_map<std::string, Mesh*>& meshes, unordered_map<string, Texture2D*>& textures, string& meshPath, string& texturePath) {
	{
		Hitbox* leftLeg = new Hitbox(glm::vec3(-0.5f, -3.f, 0.f), glm::vec3(0.5f, 1.f, 0.5f));
		bodyParts.push_back({ leftLeg, glm::vec3(0, 0.66, 0.66) });

		Hitbox* rightLeg = new Hitbox(glm::vec3(0.5f, -3.f, 0.f), glm::vec3(0.5f, 1.f, 0.5f));
		bodyParts.push_back({ rightLeg, glm::vec3(0, 0.66, 0.66) });

		Hitbox* body = new Hitbox(glm::vec3(0.f, -1.5f, 0.f), glm::vec3(1.5f, 1.5f, 0.5f));
		bodyParts.push_back({ body, glm::vec3(0.312, 0.671, 0.675) });

		Hitbox* leftArm = new Hitbox(glm::vec3(-1.0f, -1.5f, 0.f), glm::vec3(0.5f, 1.5f, 0.5f));
		bodyParts.push_back({ leftArm, glm::vec3(0.6785, 0.808, 718) });

		Hitbox* rightArm = new Hitbox(glm::vec3(1.0f, -1.5f, 0.f), glm::vec3(0.5f, 1.5f, 0.5f));
		bodyParts.push_back({ rightArm, glm::vec3(0.6785, 0.808, 718) });

		Hitbox* head = new Hitbox(glm::vec3(0.f, -0.5f, 0.f), glm::vec3(0.75f));
		bodyParts.push_back({ head, glm::vec3(0.6785, 0.808, 718) });
	}

	Mesh* gun = new Mesh("shotgun");
	gun->LoadMesh(meshPath, "Shotgun.obj");
	meshes[gun->GetMeshID()] = gun;

	Texture2D* gunTexture = new Texture2D();
	gunTexture->Load2D(texturePath.c_str(), GL_REPEAT);
	textures["shotgun"] = gunTexture;
}

void Player::handleInput(float deltaTimeSeconds, WindowObject* window, MyCamera* camera, vector<Bullet*>& bullets, TileMap* tileMap) {
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		camera->switchToFirstPerson();
	} else {
		camera->switchToThirdPerson();
	}

	timeSinceLastBullet += deltaTimeSeconds;
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		if (timeSinceLastBullet >= fireRate) {
			bullets.push_back(new Bullet(camera->getPosition(), camera->getForward()));
			timeSinceLastBullet = 0;
		}
	}

	collidesBack = false;
	collidesFront = false;
	collidesLeft = false;
	collidesRight = false;

	auto layout = tileMap->getLayout();

	for (int i = 0; i < layout.size(); i++) {
		for (int j = 0; j < layout[i].size(); j++) {
			if (layout[i][j]->value == 1) {
				auto w = layout[i][j]->hitbox;

				if (window->KeyHold(GLFW_KEY_W)) {
					glm::vec3 dir = glm::normalize(glm::vec3(camera->getForward().x, 0, camera->getForward().z));
					glm::vec3 newPos = hitbox->pos + dir * moveSpeed * deltaTimeSeconds;
					if (newPos.x + hitbox->dimensions.x / 2 >= w->pos.x && newPos.x + hitbox->dimensions.x / 2 <= w->pos.x + w->dimensions.x * 3/2
						&& newPos.z + hitbox->dimensions.z / 2 >= w->pos.z && newPos.z + hitbox->dimensions.z / 2 <= w->pos.z + w->dimensions.z * 3/2) {
						collidesFront = true;
					}
				}

				if (window->KeyHold(GLFW_KEY_S)) {
					glm::vec3 dir = glm::normalize(glm::vec3(camera->getForward().x, 0, camera->getForward().z));
					glm::vec3 newPos = hitbox->pos - dir * moveSpeed * deltaTimeSeconds;
					if (newPos.x + hitbox->dimensions.x / 2 >= w->pos.x && newPos.x + hitbox->dimensions.x / 2 <= w->pos.x + w->dimensions.x * 3 / 2
						&& newPos.z + hitbox->dimensions.z / 2 >= w->pos.z && newPos.z + hitbox->dimensions.z / 2 <= w->pos.z + w->dimensions.z * 3 / 2) {
						collidesBack = true;
					}
				}

				if (window->KeyHold(GLFW_KEY_A)) {
					glm::vec3 dir = glm::normalize(glm::vec3(camera->getRight().x, 0, camera->getRight().z));
					glm::vec3 newPos = hitbox->pos - dir * moveSpeed * deltaTimeSeconds;
					if (newPos.x + hitbox->dimensions.x / 2 >= w->pos.x && newPos.x + hitbox->dimensions.x / 2 <= w->pos.x + w->dimensions.x * 3 / 2
						&& newPos.z + hitbox->dimensions.z / 2 >= w->pos.z && newPos.z + hitbox->dimensions.z / 2 <= w->pos.z + w->dimensions.z * 3 / 2) {
						collidesLeft = true;
					}
				}

				if (window->KeyHold(GLFW_KEY_D)) {
					glm::vec3 dir = glm::normalize(glm::vec3(camera->getRight().x, 0, camera->getRight().z));
					glm::vec3 newPos = hitbox->pos + dir * moveSpeed * deltaTimeSeconds;
					if (newPos.x + hitbox->dimensions.x / 2 >= w->pos.x && newPos.x + hitbox->dimensions.x / 2 <= w->pos.x + w->dimensions.x * 3 / 2
						&& newPos.z + hitbox->dimensions.z / 2 >= w->pos.z && newPos.z + hitbox->dimensions.z / 2 <= w->pos.z + w->dimensions.z * 3 / 2) {
						collidesRight = true;
					}
				}
			}
		}
	}

	if (window->KeyHold(GLFW_KEY_W) && !collidesFront) {
		glm::vec3 dir = glm::normalize(glm::vec3(camera->getForward().x, 0, camera->getForward().z));
		hitbox->pos += dir * moveSpeed * deltaTimeSeconds;
	}

	if (window->KeyHold(GLFW_KEY_S) && !collidesBack) {
		glm::vec3 dir = glm::normalize(glm::vec3(camera->getForward().x, 0, camera->getForward().z));
		hitbox->pos -= dir * moveSpeed * deltaTimeSeconds;
	}

	if (window->KeyHold(GLFW_KEY_A) && !collidesLeft) {
		glm::vec3 dir = glm::normalize(glm::vec3(camera->getRight().x, 0, camera->getRight().z));
		hitbox->pos -= dir * moveSpeed * deltaTimeSeconds;
	}

	if (window->KeyHold(GLFW_KEY_D) && !collidesRight) {
		glm::vec3 dir = glm::normalize(glm::vec3(camera->getRight().x, 0, camera->getRight().z));
		hitbox->pos += dir * moveSpeed * deltaTimeSeconds;
	}

	if (window->KeyHold(GLFW_KEY_SPACE)) {
		glm::vec3 dir = glm::normalize(glm::vec3(0, camera->getUp().y, 0));
		hitbox->pos += dir * deltaTimeSeconds * moveSpeed;
	}

	if (window->KeyHold(GLFW_KEY_LEFT_SHIFT)) {
		glm::vec3 dir = glm::normalize(glm::vec3(0, camera->getUp().y, 0));
		hitbox->pos -= dir * deltaTimeSeconds * moveSpeed;
	}
}

Hitbox* Player::getHitbox() {
	return hitbox;
}

std::vector<std::pair<Hitbox*, glm::vec3>> Player::getBodyParts() {
	return bodyParts;
}

ResourceContainer* Player::getHealth() {
	return health;
}

ResourceContainer* Player::getTimer() {
	return escapeTimer;
}