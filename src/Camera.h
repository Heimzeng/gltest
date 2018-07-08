#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>

//glm
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Camera {
public:
    Camera() {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        direction = glm::vec3(0.0f, 0.0f, -1.0f);
        directionChanged = false;
        rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    /* Executes glRotate and glTranslate.
     * Should be called after calling glLoadIdentity. */
    void render(void) const;
    void move(const glm::vec3 & direction);
    void moveX(float distance);
    void moveY(float distance);
    void moveZ(float distance);
    void rotate(const glm::vec3 & angles);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
	glm::vec3 getPosition() const;
	glm::vec3 getDirection() const;
	glm::vec3 getRotation() const;
    void updateDirection(void);
private:
	glm::vec3 position;
    /* needs to mark the update by setting directionChanged to true
     * before calling any move function */
	glm::vec3 direction;
	glm::vec3 rotation;
    bool directionChanged;
};

#endif
