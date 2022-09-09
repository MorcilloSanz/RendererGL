#include "Camera.h"

Camera::Camera(const glm::mat4& _projectionMatrix, const glm::mat4& _viewMatrix) 
    : projectionMatrix(_projectionMatrix), viewMatrix(_viewMatrix), eye(glm::vec3(0, 0, 0)) {
}

void Camera::lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
    this->eye = eye;
    this->center = center;
    this->up = up;
    viewMatrix = glm::lookAt(eye, center, up);
}