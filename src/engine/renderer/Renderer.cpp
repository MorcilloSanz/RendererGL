#include "Renderer.h"

#include "../../../glew/glew.h"

Renderer::Renderer() : camera(nullptr), hasCamera(false), light(nullptr), hasLight(false) {
    Shader vertexShader = Shader::fromFile("../src/engine/opengl/glsl/vertex_shader.glsl", Shader::ShaderType::Vertex);
    Shader fragmentShader = Shader::fromFile("../src/engine/opengl/glsl/fragment_shader.glsl", Shader::ShaderType::Fragment);
    shaderProgram = std::make_shared<ShaderProgram>(vertexShader, fragmentShader);
    enableBlending();
    enableAntialiasing();
}

void Renderer::enableBlending() {
    glEnable(GL_BLEND & GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::enableAntialiasing() {
    glEnable(GL_MULTISAMPLE);
}

void Renderer::enableBackFaceCulling() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); 
}

void Renderer::enableFrontFaceCulling() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW); 
}

void Renderer::setCamera(Camera& camera) {
    hasCamera = true;
    this->camera = &camera;
}

void Renderer::setLight(Light& light) {
    hasLight = true;
    this->light = &light;
}

void Renderer::render() {
    
    if(!hasLight) shaderProgram->useProgram();
    else    light->getShaderProgram()->useProgram();

    glm::mat4 projection(1.f);
    glm::mat4 view(1.f);
    if(hasCamera) {
        projection = camera->getProjectionMatrix();
        view = camera->getViewMatrix();
    }

    auto textureUniform = [&](std::shared_ptr<ShaderProgram>& shaderProgram, std::shared_ptr<Polytope>& polytope) {
        unsigned int index = 0;
        shaderProgram->uniformInt("nTextures", polytope->getTextures().size());
        for(auto& texture : polytope->getTextures()) {
            texture->bind();
            std::vector<int> textures;
            for(int i = 0; i < index + 1; i ++) textures.push_back(polytope->getTextures()[i]->getID() - 1);
            shaderProgram->uniformTextureArray("textures", textures);
            index ++;
        }
    };

    for(Group* group : groups) {
        if(group->isVisible()) {
            // Primitive settings
            glPointSize(group->getPointSize());
            glLineWidth(group->getLineWidth());
            // Draw call
            for(auto& polytope : group->getPolytopes()) {
                // Calculate model view matrix
                glm::mat4 model = group->getModelMatrix() * polytope->getModelMatrix();
                glm::mat4 mvp = projection * view * model;
                // Send to vertex shader
                if(!hasLight) {
                    shaderProgram->uniformMat4("mvp", mvp);
                    textureUniform(shaderProgram, polytope);
                } 
                else if(hasCamera) {
                    // Light
                    light->getShaderProgram()->uniformVec3("light.position", light->getPosition());
                    light->getShaderProgram()->uniformVec3("light.color", light->getColor());
                    light->getShaderProgram()->uniformVec3("light.ambient", light->getAmbientColor());
                    light->getShaderProgram()->uniformVec3("light.diffuse", light->getDiffuseColor());
                    light->getShaderProgram()->uniformVec3("light.specular", light->getSpecularColor());
                    light->getShaderProgram()->uniformVec3("viewPos", camera->getEye());
                    // Material
                    light->getShaderProgram()->uniformVec3("material.diffuse", polytope->getMaterial().getDiffuse());
                    light->getShaderProgram()->uniformVec3("material.specular", polytope->getMaterial().getSpecular());
                    light->getShaderProgram()->uniformFloat("material.shininess", polytope->getMaterial().getShininess());
                    // Texture
                    textureUniform(light->getShaderProgram(), polytope);
                    // Model view projection
                    light->getShaderProgram()->uniformMat4("model", model);
                    light->getShaderProgram()->uniformMat4("view", view);
                    light->getShaderProgram()->uniformMat4("projection", projection);
                }
                // Draw
                polytope->draw(group->getPrimitive(), group->isShowWire());
                // unbind textures
                for(auto& texture : polytope->getTextures()) texture->unbind();
            }
        }
    }
}

void Renderer::clear() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}