#pragma once

#include <iostream>
#include <memory>

#include "../../../glm/vec3.hpp"
#include "../../../glm/vec4.hpp"
#include "../../../glm/mat4x4.hpp"
#include "../../../glm/gtc/type_ptr.hpp"

#include "../opengl/shader/Shader.h"

class Light {
private:
    glm::vec3 position;
    glm::vec3 color;
    float ambientStrength, diffuseStrength, specularStrength;
    std::shared_ptr<ShaderProgram> shaderProgram;
public:
    Light(const glm::vec3& position);
    Light(const glm::vec3& position, const glm::vec3& color);
    Light(const glm::vec3& _position, const glm::vec3& _color, float _ambientStrength, float _diffuseStrength, float _specularStrength);
    Light() = default;
    ~Light() = default;
public:
    inline void setPosition(const glm::vec3 position) { this->position = position; }
    inline glm::vec3& getPosition() { return position; }

    inline void setColor(const glm::vec3& color) { this->color = color; }
    inline glm::vec3& getColor() { return color; }

    inline void setAmbientStrength(float ambientStrength) { this->ambientStrength = ambientStrength; }
    inline float getAmbientStrength() const { return ambientStrength; }

    inline void setDiffuseStrength(float diffuseStrength) { this->diffuseStrength = diffuseStrength; }
    inline float getDiffuseStrength() const { return diffuseStrength; }

    inline void setSpecularStrength(const float specularStrength) { this->specularStrength = specularStrength; }
    inline float getSpecularStrength() const { return specularStrength; }

    inline glm::vec3 getDiffuseColor() { return color * glm::vec3(diffuseStrength); }
    inline glm::vec3 getAmbientColor() { return getDiffuseColor() * glm::vec3(ambientStrength); }
    inline glm::vec3 getSpecularColor() { return color * glm::vec3(specularStrength); }

    inline std::shared_ptr<ShaderProgram>& getShaderProgram() { return shaderProgram; }
};