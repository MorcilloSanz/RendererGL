// Thanks to: https://antongerdelan.net/opengl/raycasting.html
#pragma once

#include "Camera.h"

class MouseRayCasting {
public:
    /**
     * r : (x, y, z) = (x0, y0, z0) + lambda * (v1, v2, v3)
     * Where (x0, y0, z0) is the origin of the ray
     * and   (v1, v2, v3) is the direction vector of the ray
     */
    struct Ray {
        glm::vec3 rayDirection;
        glm::vec3 origin;

        Ray(const glm::vec3& _origin, const glm::vec3& _rayDirection)
            : origin(_origin), rayDirection(_rayDirection) {
        }
        Ray() = default;
        ~Ray() = default;

        glm::vec3 getPoint(double lambda) {
            return glm::vec3(
                origin.x + rayDirection.x * lambda,
                origin.y + rayDirection.y * lambda,
                origin.z + rayDirection.z * lambda
            );
        }

        /**
         * @brief Returns a 3D point which projection belongs to the Screen Plane (X, Y)
         * @return glm::vec3 
         */
        inline glm::vec3 getScreenProjectedPoint() {
            return getPoint(1);
        }
    };
private:
    Camera camera;
    unsigned int width, height;
public:
    MouseRayCasting(const Camera& _camera, unsigned int _width, unsigned int _height);
    MouseRayCasting() = default;
    ~MouseRayCasting() = default;
private:
    glm::vec2 getNormalizedDeviceCoords(const glm::vec2& mousePosition);
    glm::vec4 getHomogeneousClipCoords(const glm::vec2& normalizedDeviceCoords);
    glm::vec4 getEyeCoords(const glm::vec4& clipCoords);
    glm::vec3 getWorldCoords(const glm::vec4& eyeCoords);
public:
    Ray getRay(int mouseX, int mouseY);
};