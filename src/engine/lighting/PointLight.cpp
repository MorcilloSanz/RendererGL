#include "PointLight.h"

PointLight::PointLight(const glm::vec3& position) 
    : PointLight(position, glm::vec3(1)) {
}

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color) 
    : PointLight(position, color, CONSTANT, LINEAR, QUADRATIC) {
}

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float _constant, float _linear, float _quadratic) 
    : Light(position, color), constant(_constant), linear(_linear), quadratic(_quadratic) {
}