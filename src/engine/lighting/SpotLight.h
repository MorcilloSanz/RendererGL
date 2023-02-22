#pragma once

#include "PointLight.h"

#define CUTOFF 12.5f
#define OUTERCUTOFF 17.5f

class SpotLight : public PointLight {
    GENERATE_PTR(SpotLight)
private:
    glm::vec3 direction;
    float cutOff, outerCutOff;
public:
    SpotLight(const glm::vec3& position, const glm::vec3& direction);
    SpotLight(const glm::vec3& position, const glm::vec3& direction, float cutOff, float outerCutOff);
    SpotLight(const glm::vec3& position, const glm::vec3& _direction, float _cutOff, float _outerCutOff, const glm::vec3& color);
    SpotLight() = default;
    ~SpotLight() = default;
public:
    inline void setDirection(const glm::vec3& direction) { this->direction = direction; }
    inline glm::vec3& getDirection() { return direction; }
    /**
     * @brief Set Cut Off
     * IN RADIANS
     * @param cutOff 
     */
    inline void setCutOff(float cutOff) { this->cutOff = cutOff; }
    inline float getCutOff() const { return cutOff; }
    /**
     * @brief Set Outer Cut Off 
     * IN RADIANS
     * @param cutOff 
     */
    inline void setOuterCutOff(float outerCutOff) { this-> outerCutOff = outerCutOff;}
    inline float getOuterCutOff() const { return outerCutOff; }
};