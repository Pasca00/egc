#include "lab_m1/tema 3/Lights/Globe.h"
#include "utils/text_utils.h"

Globe::Globe(glm::vec3 position) {
	this->position = position;
	this->scale = glm::vec3(2.f);

	this->resolution = 16 * 16 * 4;

	this->active = true;

	this->data = new unsigned char[resolution];
	for (int i = 0; i < resolution; i++) {
		if (i % 4 == 3) {
			data[i] = 255;
		} else {
			data[i] = rand() % 256;
		}
	}

	this->texture = new Texture2D();
	this->texture->Create(data, 16, 16, 4);
	this->texture->SetFiltering(GL_NEAREST, GL_NEAREST);
}

void Globe::changeState() {
	active = !active;
}

glm::vec3 Globe::getPosition() {
	return position;
}

glm::vec3 Globe::getScale() {
	return scale;
}

unsigned char* Globe::getData() {
	return data;
}

bool Globe::isActive() {
	return active;
}

Texture2D* Globe::getTexture() {
	return texture;
}
