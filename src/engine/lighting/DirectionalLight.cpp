#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& direction) 
    : Light(direction) {
}

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color) 
    : Light(direction, color) {
}

void DirectionalLight::setDirection(const glm::vec3& direction) {
    this->direction = direction;
    position = direction;
}