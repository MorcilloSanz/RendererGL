#include "SpotLight.h"

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction) 
    : SpotLight(position, direction, glm::radians(CUTOFF), glm::radians(OUTERCUTOFF)) {
}

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, float cutOff, float outerCutOff) 
    : SpotLight(position, direction, cutOff, outerCutOff, glm::vec3(1)) {
}

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& _direction, float _cutOff, float _outerCutOff, const glm::vec3& color) 
    : PointLight(position, color), direction(_direction), cutOff(_cutOff), outerCutOff(_outerCutOff) {
}