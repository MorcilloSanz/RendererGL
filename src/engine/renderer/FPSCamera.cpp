#include "FPSCamera.h"

#include <iostream>

#define DEFAULT_SENSITIVITY 0.1
#define DEFAULT_CAMERA_SPEED 0.1

FPSCamera::FPSCamera(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) 
    : Camera(projectionMatrix, viewMatrix), cameraFront(glm::vec3(0, 0, -1)), yaw(-90.0f), pitch(0),
    lastX(0), lastY(0), sensitivity(DEFAULT_SENSITIVITY), cameraSpeed(DEFAULT_CAMERA_SPEED) {
    center = glm::vec3(0, 0, 0);
    up = glm::vec3(0, -1, 0);
    eye = glm::vec3(0, 0, 1);
	this->viewMatrix = glm::lookAt(eye, center, up);
}

FPSCamera::Ptr FPSCamera::orthoCamera(float left, float right, float bottom, float top, float zNear, float zFar) {

    glm::mat4 projection = glm::ortho(left, right, bottom, top, zNear, zFar);
    glm::mat4 view(1.0f);

    FPSCamera::Ptr camera = FPSCamera::New(projection, view);

    return camera;
}

FPSCamera::Ptr FPSCamera::perspectiveCamera(float fovy, float aspect, float zNear, float zFar) {

    glm::mat4 projection = glm::perspective(fovy, aspect, zNear, zFar);
    glm::mat4 view(1.0f);

    FPSCamera::Ptr camera = FPSCamera::New(projection, view);

    return camera;
}

glm::mat4& FPSCamera::getViewMatrix() {
    viewMatrix = glm::lookAt(eye, eye + cameraFront, up);
	return viewMatrix;
}

void FPSCamera::move(const Movement& movement) {
    switch (movement) {
        case Movement::Forward:
            eye += cameraSpeed * cameraFront;
        break;
        case Movement::Backward:
            eye -= cameraSpeed * cameraFront;
        break;
        case Movement::Right:
            eye += glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;
        break;
        case Movement::Left:
            eye -= glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;
        break;
        default:
        break;
	}
}

void FPSCamera::lookAround(double xpos, double ypos) {
    double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw -= xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}