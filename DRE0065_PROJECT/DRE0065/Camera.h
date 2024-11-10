#ifndef CAMERA_H
#define CAMERA_H

#include "Subject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera : public Subject
{
public:
    Camera(vec3 position, vec3 up, float yaw, float pitch);
    mat4 getViewMatrix();
    void processKeyboard(int direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void resetPosition(const vec3& startPosition, float startYaw, float startPitch);
    vec3 position, front, up, right, worldUp;
    float yaw, pitch, movementSpeed, mouseSensitivity;

private:
    void updateCameraVectors();
    mat4 viewMatrix;
    bool viewMatrixUpdated;
};
#endif