#include "Renderer.h"

#include "../../../glew/glew.h"

#include "../opengl/shader/Program.h"

Renderer::Renderer() 
    : camera(nullptr), hasCamera(false), light(nullptr), hasLight(false),
    projection(glm::mat4(1.f)), view(glm::mat4(1.f)) {
    Shader vertexShader(Program::getVertexShaderCode(), Shader::ShaderType::Vertex);
    Shader fragmentShader(Program::getFragmentShaderCode(), Shader::ShaderType::Fragment);
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

void Renderer::removeGroup(Group& group) {
    unsigned int index = 0;
    for(Group* g : groups) {
        if(g == &group) {
            removeGroup(index);
            break;
        }
        index ++;
    }
}

void Renderer::textureUniform(std::shared_ptr<ShaderProgram>& shaderProgram, std::shared_ptr<Polytope>& polytope) {
    unsigned int index = 0;
    shaderProgram->uniformInt("nTextures", polytope->getTextures().size());
    for(auto& texture : polytope->getTextures()) {
        texture->bind();
        std::vector<int> textures;
        for(int i = 0; i < index + 1; i ++) textures.push_back(polytope->getTextures()[i]->getID() - 1);
        shaderProgram->uniformTextureArray("textures", textures);
        index ++;
    }
}

void Renderer::primitiveSettings(Group* group) {
    glPointSize(group->getPointSize());
    glLineWidth(group->getLineWidth());
}

void Renderer::lightShaderUniforms() {
    light->getShaderProgram()->uniformVec3("light.position", light->getPosition());
    light->getShaderProgram()->uniformVec3("light.color", light->getColor());
    light->getShaderProgram()->uniformVec3("light.ambient", light->getAmbientColor());
    light->getShaderProgram()->uniformVec3("light.diffuse", light->getDiffuseColor());
    light->getShaderProgram()->uniformVec3("light.specular", light->getSpecularColor());
    light->getShaderProgram()->uniformVec3("viewPos", camera->getEye());
}

void Renderer::lightMaterialUniforms(const std::shared_ptr<Polytope>& polytope) {
    light->getShaderProgram()->uniformVec3("material.diffuse", polytope->getMaterial().getDiffuse());
    light->getShaderProgram()->uniformVec3("material.specular", polytope->getMaterial().getSpecular());
    light->getShaderProgram()->uniformFloat("material.shininess", polytope->getMaterial().getShininess());
}

void Renderer::lightMVPuniform(const glm::mat4& model) {
    light->getShaderProgram()->uniformMat4("model", model);
    light->getShaderProgram()->uniformMat4("view", view);
    light->getShaderProgram()->uniformMat4("projection", projection);
}

void Renderer::drawGroup(Group* group) {
    if(!group->isVisible()) return;

    primitiveSettings(group);
    
    for(auto& polytope : group->getPolytopes()) {
        // Uniforms
        glm::mat4 model = group->getModelMatrix() * polytope->getModelMatrix();
        if(!hasLight) {
            glm::mat4 mvp = projection * view * model;
            shaderProgram->uniformMat4("mvp", mvp);
            textureUniform(shaderProgram, polytope);
        } 
        else if(hasCamera) {
            lightShaderUniforms();
            lightMaterialUniforms(polytope);
            textureUniform(light->getShaderProgram(), polytope);
            lightMVPuniform(model);
        }
        // Draw
        polytope->draw(group->getPrimitive(), group->isShowWire());
        // unbind textures
        for(auto& texture : polytope->getTextures()) texture->unbind();
    }
}

void Renderer::drawSkyBox() {
    if(skyBox == nullptr) return;
    skyBox->getShaderProgram()->useProgram();
    // remove translation from the view matrix
    view = glm::mat4(glm::mat3(camera->getViewMatrix())); 
    skyBox->getShaderProgram()->uniformInt("skybox", 0);
    skyBox->getShaderProgram()->uniformMat4("view", view);
    skyBox->getShaderProgram()->uniformMat4("projection", projection);
    // Draw call
    glDepthRange(0.999,1.0);
    skyBox->draw();
    glDepthRange(0.0,1.0);
     // set depth function back to default
    glDepthFunc(GL_LESS);
}

void Renderer::render() {
    enableAntialiasing();
    enableBlending();
    // Use shader programs
    if(!hasLight)   shaderProgram->useProgram();
    else            light->getShaderProgram()->useProgram();
    // Init transform matrices
    if(hasCamera) {
        projection = camera->getProjectionMatrix();
        view = camera->getViewMatrix();
    }
    // Draw groups
    for(Group* group : groups) drawGroup(group);
    // Draw skybox
    drawSkyBox();
}

void Renderer::setBackgroundColor(float r, float g, float b) {
    backgroundColor.r = r;
    backgroundColor.g = g;
    backgroundColor.b = b;
}

void Renderer::clear() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}