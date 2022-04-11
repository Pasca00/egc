#pragma once

#include "lab_m1/tema 1/Map/WorldObject.h"

#define X_DIR_RIGHT  1
#define X_DIR_LEFT  -1
#define Y_DIR_DOWN  -1
#define Y_DIR_UP     1
#define DIR_NONE	 0

class Movable {
	protected:
		float moveSpeed;

		Rect bounds;

		bool leftCollision;
		bool rightCollision;
		bool upCollision;
		bool downCollision;

	public:
		int dmg;
		float rotationAngle;
		char xDirection;
		char yDirection;

		Movable(float x, float y, float w, float h);

		bool collidesLeft();
		bool collidesRight();
		bool collidesUp();
		bool collidesDown();

		void clearCollisions();

		void setLeftCollision(bool c);
		void setRightCollision(bool c);
		void setUpCollision(bool c);
		void setDownCollision(bool c);

		Rect getBounds();
		virtual float getMoveSpeed();

		virtual void handleCollision(Movable* e) = 0;
};