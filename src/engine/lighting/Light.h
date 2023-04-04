#pragma once

#include <iostream>
#include <memory>

#include "../../../glm/vec3.hpp"
#include "../../../glm/vec4.hpp"
#include "../../../glm/mat4x4.hpp"
#include "../../../glm/gtc/type_ptr.hpp"

#include "../opengl/shader/Shader.h"

#include "../ptr.h"

#define AMBIENT_STRENGTH 0.5f 
#define DIFFUSE_STRENGTH 0.5f
#define SPECULAR_STRENGTH 0.5f

class Light {
    GENERATE_PTR(Light)
protected:
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 ambient, diffuse, specular;
    float intensity;
public:
    static bool blinn;
public:
    Light(const glm::vec3& position);
    Light(const glm::vec3& _position, const glm::vec3& _color);
    Light() = default;
    virtual ~Light() = default;
public:
    inline void setPosition(const glm::vec3& position) { this->position = position; }
    inline glm::vec3& getPosition() { return position; }

    inline void setColor(const glm::vec3& color) { this->color = color; }
    inline glm::vec3& getColor() { return color; }

    inline void setAmbient(const glm::vec3& ambient) { this->ambient = ambient; }
    inline glm::vec3& getAmbient() { return ambient; }

    inline void setDiffuse(const glm::vec3& diffuse) { this->diffuse = diffuse; }
    inline glm::vec3& getDiffuse() { return diffuse; }

    inline void setSpecular(const glm::vec3& specular) { this->specular = specular; }
    inline glm::vec3& getSpecular() { return specular; }

    inline void setIntensity(float intensity) { this->intensity = intensity; }
    inline float getIntensity() const { return intensity; }
};