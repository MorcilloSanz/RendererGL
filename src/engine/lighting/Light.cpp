#include "Light.h"

#include "../../../glew/glew.h"

bool Light::blinn = true;

Light::Light(const glm::vec3& position)
    : Light(position, glm::vec3(1, 1, 1)) {
}

Light::Light(const glm::vec3& _position, const glm::vec3& _color) 
    : position(_position), color(_color), ambient(glm::vec3(AMBIENT_STRENGTH)),
    diffuse(glm::vec3(DIFFUSE_STRENGTH)), specular(glm::vec3(SPECULAR_STRENGTH)),
    intensity(1.0f) {
}