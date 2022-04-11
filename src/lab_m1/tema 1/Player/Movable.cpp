#include "lab_m1/tema 1/Player/Movable.h"

Movable::Movable(float x, float y, float w, float h)
	: bounds(x, y, w, h) {

	moveSpeed = 180;
	xDirection = X_DIR_RIGHT;
	yDirection = Y_DIR_UP;

	rotationAngle = 0;
	dmg = 0;

	leftCollision  = false;
	rightCollision = false;
	upCollision    = false;
	downCollision  = false;
}

bool Movable::collidesLeft() {
	return leftCollision;
}

bool Movable::collidesRight() {
	return rightCollision;
}

bool Movable::collidesUp() {
	return upCollision;
}

bool Movable::collidesDown() {
	return downCollision;
}

void Movable::clearCollisions() {
	leftCollision = false;
	rightCollision = false;
	upCollision = false;
	downCollision = false;
}

void Movable::setLeftCollision(bool c) {
	leftCollision = c;
}

void Movable::setRightCollision(bool c) {
	rightCollision = c;
}

void Movable::setUpCollision(bool c) {
	upCollision = c;
}

void Movable::setDownCollision(bool c) {
	downCollision = c;
}

Rect Movable::getBounds() {
	return bounds;
}

float Movable::getMoveSpeed() {
	return moveSpeed;
}