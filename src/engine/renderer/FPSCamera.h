#pragma once

#include "Camera.h"

class FPSCamera : public Camera {
    GENERATE_PTR(FPSCamera)
public:
    enum class Movement {
		Forward, Backward, Right, Left
	};
private:
    glm::vec3 cameraFront;
	unsigned int width, height;
	float lastX, lastY;
	float yaw, pitch;
    float sensitivity, cameraSpeed;
public:

    FPSCamera(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);

    FPSCamera() = default;
    ~FPSCamera() = default;

    static FPSCamera::Ptr orthoCamera(float left, float right, float bottom, float top, float zNear, float zFar);
    static FPSCamera::Ptr perspectiveCamera(float fovy, float aspect, float zNear, float zFar);

    virtual glm::mat4& getViewMatrix() override;
    virtual void move(const Movement& movement);
	virtual void lookAround(double xpos, double ypos);

    inline void setSensitivity(float sensitivity) { this->sensitivity = sensitivity; }
    inline float getSensitivity() const { return sensitivity; }

    inline void setCameraSpeed(float cameraSpeed) { this->cameraSpeed = cameraSpeed; }
    inline float getCameraSpeed() const { return cameraSpeed; }

    inline void setCameraFront(const glm::vec3& cameraFront) { this->cameraFront = cameraFront; }
    inline glm::vec3& getCameraFront() { return cameraFront; }

    inline unsigned int getWidth() const { return width; }
    inline void setWidth(unsigned int width) { this->width = width; }

    inline unsigned int getHeight() const { return height; }
    inline void setHeight(unsigned int height) { this->height = height; }
};