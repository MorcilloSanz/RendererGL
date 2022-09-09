#include "Light.h"

#include "../../../glew/glew.h"

#define AMBIENT_STRENGTH 0.5f 
#define DIFFUSE_STRENGTH 0.5f
#define SPECULAR_STRENGTH 0.5f

Light::Light(const glm::vec3& position)
    : Light(position, glm::vec3(1, 1, 1)) {
}

Light::Light(const glm::vec3& position, const glm::vec3& color) 
    : Light(position, color, AMBIENT_STRENGTH, DIFFUSE_STRENGTH, SPECULAR_STRENGTH) {
}

Light::Light(const glm::vec3& _position, const glm::vec3& _color, float _ambientStrength, float _diffuseStrength, float _specularStrength) 
    : position(_position), color(_color), ambientStrength(_ambientStrength), diffuseStrength(_diffuseStrength), specularStrength(_specularStrength) {
    Shader vertexShader = Shader::fromFile("../src/engine/opengl/glsl/phong_vertex.glsl", Shader::ShaderType::Vertex);
    Shader fragmentShader = Shader::fromFile("../src/engine/opengl/glsl/phong_fragment.glsl", Shader::ShaderType::Fragment);
    shaderProgram = std::make_shared<ShaderProgram>(vertexShader, fragmentShader);
}