#include "Renderer.h"

#include "../../../glew/glew.h"

#include "../opengl/shader/Program.h"

Renderer::Renderer() 
    : camera(nullptr), hasCamera(false), hasLight(false), nLights(0),
    projection(glm::mat4(1.f)), view(glm::mat4(1.f)) {
    initShaders();
    enableBlending();
    enableAntialiasing();
}

void Renderer::initShaders() {
    // Default shader program
    Shader vertexShader(Program::getVertexShaderCode(), Shader::ShaderType::Vertex);
    Shader fragmentShader(Program::getFragmentShaderCode(), Shader::ShaderType::Fragment);
    shaderProgram = std::make_shared<ShaderProgram>(vertexShader, fragmentShader);
    // Lighting shader program
    Shader vertexLightingShader(Program::getPhongVertexShaderCode(), Shader::ShaderType::Vertex);
    Shader fragmentLightingShader(Program::getPhongFragmentShaderCode(), Shader::ShaderType::Fragment);
    shaderProgramLighting = std::make_shared<ShaderProgram>(vertexLightingShader, fragmentLightingShader);
    // Selection shader program
    Shader vertexSelectionShader(Program::getOutlineVertexShaderCode(), Shader::ShaderType::Vertex);
    Shader fragmentSelectionShader(Program::getOutlineFragmentShaderCode(), Shader::ShaderType::Fragment);
    shaderProgramSelection = std::make_shared<ShaderProgram>(vertexSelectionShader, fragmentSelectionShader);
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
    shaderProgramLighting->uniformInt("nLights", nLights);
    for(int i = 0; i < nLights; i ++) {
        std::string lightUniform = "lights[" + std::to_string(i) + "]";
        shaderProgramLighting->uniformVec3(lightUniform + ".position", lights[i]->getPosition());
        shaderProgramLighting->uniformVec3(lightUniform + ".color", lights[i]->getColor());
        shaderProgramLighting->uniformVec3(lightUniform + ".ambient", lights[i]->getAmbientColor());
        shaderProgramLighting->uniformVec3(lightUniform + ".diffuse", lights[i]->getDiffuseColor());
        shaderProgramLighting->uniformVec3(lightUniform + ".specular", lights[i]->getSpecularColor());
    }
    shaderProgramLighting->uniformInt("blinn", Light::blinn);
    shaderProgramLighting->uniformInt("gammaCorrection", Light::gammaCorrection);
    shaderProgramLighting->uniformVec3("viewPos", camera->getEye());
}

void Renderer::lightMaterialUniforms(const std::shared_ptr<Polytope>& polytope) {
    shaderProgramLighting->uniformVec3("material.diffuse", polytope->getMaterial().getDiffuse());
    shaderProgramLighting->uniformVec3("material.specular", polytope->getMaterial().getSpecular());
    shaderProgramLighting->uniformFloat("material.shininess", polytope->getMaterial().getShininess());
}

void Renderer::lightMVPuniform(const glm::mat4& model) {
    shaderProgramLighting->uniformMat4("model", model);
    shaderProgramLighting->uniformMat4("view", view);
    shaderProgramLighting->uniformMat4("projection", projection);
}

void Renderer::drawGroup(Group* group) {
    if(!group->isVisible()) return;

    primitiveSettings(group);
    
    for(auto& polytope : group->getPolytopes()) {
        // Compute model matrix from polytope and group
        glm::mat4 model = group->getModelMatrix() * polytope->getModelMatrix();
        glm::mat4 mvp = projection * view * model;
        // Use shader programs
        if(!hasLight)   shaderProgram->useProgram();
        else            shaderProgramLighting->useProgram();
        // Uniforms
        if(!hasLight) {
            shaderProgram->uniformMat4("mvp", mvp);
            textureUniform(shaderProgram, polytope);
        } 
        else if(hasCamera) {
            lightShaderUniforms();
            lightMaterialUniforms(polytope);
            textureUniform(shaderProgramLighting, polytope);
            lightMVPuniform(model);
        }
        // Draw polytope
        polytope->draw(group->getPrimitive(), group->isShowWire());
        if(polytope->isSelected()) {
            shaderProgramSelection->useProgram();
            shaderProgramSelection->uniformMat4("mvp", mvp);
            glLineWidth(group->getOutliningWidth());
            polytope->draw(group->getPrimitive(), true);
            glLineWidth(group->getLineWidth());
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
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

void Renderer::setCamera(Camera& camera) {
    hasCamera = true;
    this->camera = &camera;
}

void Renderer::addLight(Light& light) {
    hasLight = true;
    lights.push_back(&light);
    nLights ++;
}

void Renderer::clear() {
    glStencilMask(0xFF);
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::enableBlending() {
    glEnable(GL_BLEND & GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
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