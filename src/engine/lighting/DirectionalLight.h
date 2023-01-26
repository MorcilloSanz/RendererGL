#pragma once

#include "Light.h"

// Its the same as Light, but replacing position by direction. JUST NOTATION
class DirectionalLight : public Light {
private:
    glm::vec3 direction;
public:
    DirectionalLight(const glm::vec3& direction);
    DirectionalLight(const glm::vec3& direction, const glm::vec3& color);
    DirectionalLight() = default;
    ~DirectionalLight() = default;
public:
    void setDirection(const glm::vec3& direction);
public:
    inline glm::vec3& getDirection() { return direction; }
};