#include <cmath>
#include "Camera.h"

#define DEGREE 3.1415926/180

void Camera::move(const glm::vec3 &dir) {
	position += dir;
}

void Camera::rotate(const glm::vec3 &dir) {
	rotation += dir;
    directionChanged = true;
}

void Camera::moveX(GLfloat delta) {
    if (directionChanged)
        updateDirection();
	glm::vec3 moveVector;

    moveVector = glm::vec3(direction.z * -delta, 0.0f,
                   -direction.x * -delta);
	position += moveVector;
}

void Camera::moveY(GLfloat delta) {
    if (directionChanged)
        updateDirection();
    position.y += delta;
}

void Camera::moveZ(GLfloat delta) {
    if (directionChanged)
        updateDirection();
	glm::vec3 moveVector;
    moveVector = glm::vec3(direction.x * -delta,
                   direction.y * -delta,
                   direction.z * -delta);
    position += moveVector;
}


void Camera::rotateX(GLfloat angle) {
    rotation.x += angle;
    directionChanged = true;
}

void Camera::rotateY(GLfloat angle) {
    rotation.y += angle;
    directionChanged = true;
}

void Camera::render(void) const {
    glRotatef(-rotation.x, 1.0, 0.0, 0.0);
    glRotatef(-rotation.y, 0.0, 1.0, 0.0);
    glRotatef(-rotation.z, 0.0, 0.0, 1.0);
    glTranslatef(-position.x, -position.y, -position.z);
}

void Camera::updateDirection(void) {
    if (!directionChanged)
        return;
    // rotate around Y-axis
	glm::vec3 step1;
    step1 = glm::vec3(cos((rotation.y + 90.0) * DEGREE),
              0.0f, -sin((rotation.y + 90.0) * DEGREE));
    // rotate around X-axis:
    float cosX = cos(rotation.x * DEGREE);
	glm::vec3 step2;
    step2 = glm::vec3(step1.x * cosX, sin(rotation.x * DEGREE), step1.z * cosX);
    // TODO: rotate around Z-axis
    direction = step2;
    directionChanged = false;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getDirection() const {
    return direction;
}

glm::vec3 Camera::getRotation() const {
    return rotation;
}