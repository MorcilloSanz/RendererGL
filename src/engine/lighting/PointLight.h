#pragma once

#include "Light.h"

#define CONSTANT 1.0f
#define LINEAR 0.09f
#define QUADRATIC 0.032f

class PointLight : public Light {
private:
    float constant;
    float linear;
    float quadratic;
public:
    PointLight(const glm::vec3& position);
    PointLight(const glm::vec3& position, const glm::vec3& color);
    PointLight(const glm::vec3& position, const glm::vec3& color, float _constant, float _linear, float _quadratic);
    PointLight() = default;
    virtual ~PointLight() = default;
public:
    inline void setConstant(float constant) { this->constant = constant; }
    inline float getConstant() const { return constant; }
    
    inline void setLinear(float linear) { this->linear = linear; }
    inline float getLinear() const { return linear; }

    inline void setQuadratic(float quadratic) { this->quadratic = quadratic; }
    inline float getQuadratic() const { return quadratic; }
};