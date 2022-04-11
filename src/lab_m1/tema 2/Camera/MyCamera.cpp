#include "lab_m1/tema 2/Camera/MyCamera.h"

MyCamera::MyCamera(glm::vec3& position, float aspectRatio) : position(position) {
    this->forward = glm::vec3(1, 0, 0);
    this->up = glm::vec3(0, 1, 0);
    this->right = glm::vec3(0, 0, 1);
    this->distanceToTarget = 4.f;

    this->sensivityOX = 0.002f;
    this->sensivityOY = 0.002f;

    this->firstPerson = false;

    this->zNear = 5.f;
    this->aspectRatio = aspectRatio;

    this->projectionMatrix = glm::perspective(RADIANS(75.f), aspectRatio, 0.01f, 200.f);
}

MyCamera::~MyCamera() { }

glm::mat4 MyCamera::GetViewMatrix() {
    glm::mat4 viewMatrix;
    if (!firstPerson) {
        TranslateForward(-distanceToTarget);
        viewMatrix = glm::lookAt(position, position + forward, up);
        TranslateForward(distanceToTarget);
    } else {
        viewMatrix = glm::lookAt(position, position + forward, up);
    }

    return viewMatrix;
}

glm::mat4 MyCamera::GetProjectionMatrix() {
    return projectionMatrix;
}

void MyCamera::MoveForward(float distance) {
    glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    position += dir * distance;
}

void MyCamera::MoveUpward(float distance) {
    glm::vec3 dir = glm::normalize(glm::vec3(0, up.y, 0));
    position += dir * distance;
}

void MyCamera::MoveRight(float distance) {
    glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
    position += dir * distance;
}

void MyCamera::TranslateForward(float distance) {
    position += glm::normalize(forward) * distance;
}

void MyCamera::TranslateUpward(float distance) {
    position += glm::normalize(up) * distance;
}

void MyCamera::TranslateRight(float distance) {
    position += glm::normalize(right) * distance;
}

void MyCamera::RotateFirstPerson_OX(float angle) {
    angle *= -sensivityOY;
    glm::vec4 newVec = glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 0);
    forward = glm::normalize(glm::vec3(newVec));
    up = glm::normalize(glm::cross(right, forward));
}

void MyCamera::RotateFirstPerson_OY(float angle) {
    angle *= -sensivityOX;
    glm::vec4 newVec = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
    forward = glm::normalize(glm::vec3(newVec));

    newVec = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
    right = glm::normalize(glm::vec3(newVec));

    up = glm::normalize(glm::cross(right, forward));
}

void MyCamera::RotateFirstPerson_OZ(float angle) {
    glm::vec4 newVec = glm::rotate(glm::mat4(1), angle, glm::vec3(forward)) * glm::vec4(up, 1);
    up = glm::normalize(glm::vec3(newVec));
    right = glm::normalize(glm::cross(forward, up));
}

void MyCamera::RotateThirdPerson_OX(float angle) {
    RotateFirstPerson_OX(angle);
}

void MyCamera::RotateThirdPerson_OY(float angle) {
    RotateFirstPerson_OY(angle);
}

void MyCamera::RotateThirdPerson_OZ(float angle) {
    TranslateForward(distanceToTarget);
    RotateFirstPerson_OZ(angle);
    TranslateForward(-distanceToTarget);
}

void MyCamera::Rotate_OX(float angle) {
    if (firstPerson) {
        RotateFirstPerson_OX(angle);
    }
    else {
        RotateThirdPerson_OX(angle);
    }
}

void MyCamera::Rotate_OY(float angle) {
    if (firstPerson) {
        RotateFirstPerson_OY(angle);
    } else {
        RotateThirdPerson_OY(angle);
    }
}

void MyCamera::switchToFirstPerson() {
    if (!firstPerson) {
        firstPerson = true;
    }
}

void MyCamera::switchToThirdPerson() {
    if (firstPerson) {
        firstPerson = false;
    }
}

glm::vec3 MyCamera::getPosition() {
    return position;
}

glm::vec3 MyCamera::getForward() {
    return forward;
}

glm::vec3 MyCamera::getUp() {
    return up;
}

glm::vec3 MyCamera::getRight() {
    return right;
}

bool MyCamera::isInThirdPerson() {
    return !firstPerson;
}