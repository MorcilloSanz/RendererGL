#include "MouseRayCasting.h"

MouseRayCasting::MouseRayCasting(const Camera& _camera, unsigned int _width, unsigned int _height)
    : camera(_camera), width(_width), height(_height) {
}

glm::vec2 MouseRayCasting::getNormalizedDeviceCoords(const glm::vec2& mousePosition) {
    float x = (2.0f * mousePosition.x) / width - 1.0f;
    float y = (2.0f * mousePosition.y) / height - 1.0f;
    return glm::vec2(x, y);
}

glm::vec4 MouseRayCasting::getHomogeneousClipCoords(const glm::vec2& normalizedDeviceCoords) {
    return glm::vec4(normalizedDeviceCoords.x, normalizedDeviceCoords.y, -1.f, 1.f);
}

glm::vec4 MouseRayCasting::getEyeCoords(const glm::vec4& clipCoords) {
    glm::vec4 eyeCoords = camera.getInverseProjectionMatrix() * clipCoords;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0, 0.0);
    return eyeCoords;
}

glm::vec3 MouseRayCasting::getWorldCoords(const glm::vec4& eyeCoords) {
    glm::vec4 worldCoordinates = camera.getInverseViewMatrix() * eyeCoords;
    glm::vec3 worldCoords(worldCoordinates.x, worldCoordinates.y, worldCoordinates.z);
    worldCoords = glm::normalize(worldCoords);
    return worldCoords;
}

MouseRayCasting::Ray MouseRayCasting::getRay(int mouseX, int mouseY) {
    glm::vec2 normalizedDeviceCoords = getNormalizedDeviceCoords(glm::vec2(mouseX, mouseY));
    glm::vec4 clipCoords = getHomogeneousClipCoords(normalizedDeviceCoords);
    glm::vec4 eyeCoords = getEyeCoords(clipCoords);
    glm::vec3 worldCoords = getWorldCoords(eyeCoords);
    return Ray(camera.getEye(), worldCoords);
}