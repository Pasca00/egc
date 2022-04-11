#pragma once

#include "utils/glm_utils.h"

class MyCamera {
	private:
		glm::vec3& position;
		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 right;
		float distanceToTarget;

		glm::mat4 projectionMatrix;
		float zNear;
		float aspectRatio;

		float sensivityOX;
		float sensivityOY;

		bool firstPerson;

	public:
		MyCamera(glm::vec3& position, float aspectRatio);
		~MyCamera();

		void MoveForward(float distance);
		void MoveUpward(float distance);
		void MoveRight(float distance);
		void TranslateForward(float distance);
		void TranslateUpward(float distance);
		void TranslateRight(float distance);
		void RotateFirstPerson_OX(float angle);
		void RotateFirstPerson_OY(float angle);
		void RotateFirstPerson_OZ(float angle);
		void RotateThirdPerson_OX(float angle);
		void RotateThirdPerson_OY(float angle);
		void RotateThirdPerson_OZ(float angle);

		void Rotate_OX(float angle);
		void Rotate_OY(float angle);

		void switchToFirstPerson();
		void switchToThirdPerson();

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();

		glm::vec3 getPosition();
		glm::vec3 getForward();
		glm::vec3 getUp();
		glm::vec3 getRight();

		bool isInThirdPerson();
};