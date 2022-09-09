#pragma once

#include "../../../glm/vec3.hpp"
#include "../../../glm/vec4.hpp"
#include "../../../glm/mat4x4.hpp"
#include "../../../glm/gtc/type_ptr.hpp"

class Camera {
protected:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::vec3 eye, center, up;
    Camera(const glm::mat4& _projectionMatrix, const glm::mat4& _viewMatrix);
public:
    Camera() = default;
    ~Camera() = default;
public:
    virtual void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
    
    inline glm::mat4& getProjectionMatrix() { return projectionMatrix; }
    inline glm::mat4 getInverseProjectionMatrix() { return glm::inverse(projectionMatrix); }
    virtual glm::mat4& getViewMatrix() { return viewMatrix; }
    inline glm::mat4 getInverseViewMatrix() { return glm::inverse(viewMatrix); }
    inline glm::mat4 getViewProjectionMatrix() { return projectionMatrix * viewMatrix; }
    inline glm::mat4 getInverseViewProjectionMatrix() { return glm::inverse(getViewProjectionMatrix()); }

    inline void setProjectionMatrix(const glm::mat4& projectionMatrix) { this->projectionMatrix = projectionMatrix; }
    inline void setViewMatrix(const glm::mat4& viewMatrix) { this->viewMatrix = viewMatrix; } 

    virtual glm::vec3& getEye() { return eye; }
    virtual glm::vec3& getCenter() { return center; }
    virtual glm::vec3& getUp() { return up; }

    inline void setEye(const glm::vec3& eye) { this->eye = eye; }
    inline void setCenter(const glm::vec3& center) { this->center = center; }
    inline void setUp(const glm::vec3& up) { this->up = up; }
};