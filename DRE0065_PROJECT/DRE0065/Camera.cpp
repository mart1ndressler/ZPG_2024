#include "Camera.h"

Camera::Camera(vec3 position, vec3 up, float yaw, float pitch) : position(position), worldUp(up), yaw(yaw), pitch(pitch), movementSpeed(5.0f), mouseSensitivity(0.1f)
{
    front = vec3(0.0f, 0.0f, -1.0f);
    updateCameraVectors();
}

mat4 Camera::getViewMatrix()
{
    if(viewMatrixUpdated)
    {
        viewMatrix = glm::lookAt(position, position + front, up);
        viewMatrixUpdated = false;
    }
    return viewMatrix;
}

void Camera::processKeyboard(int direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    vec3 oldPosition = position;

    if(direction == 0) position += front * velocity;
    if(direction == 1) position -= front * velocity;
    if(direction == 2) position -= right * velocity;
    if(direction == 3) position += right * velocity;

    if(position != oldPosition)
    {
        viewMatrixUpdated = true;
        notifyObservers();
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;
    updateCameraVectors();
    viewMatrixUpdated = true;
    notifyObservers();
}

void Camera::resetPosition(const vec3& startPosition, float startYaw, float startPitch)
{
    position = startPosition;
    yaw = startYaw;
    pitch = startPitch;
    updateCameraVectors();
    viewMatrixUpdated = true;
    notifyObservers();
}

void Camera::updateCameraVectors()
{
    vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}