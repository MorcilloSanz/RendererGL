#include "Renderer.h"

#include "../../../glew/glew.h"

#include "../opengl/shader/Program.h"

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
   return dynamic_cast<const Base*>(ptr) != nullptr;
}

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
    // SkyBox shader program
    Shader vertexSkyBoxShader(Program::getSkyBoxVertexShaderCode(), Shader::ShaderType::Vertex);
    Shader fragmentSkyBoxShader(Program::getSkyBoxFragmentShaderCode(), Shader::ShaderType::Fragment);
    shaderProgramSkyBox = std::make_shared<ShaderProgram>(vertexSkyBoxShader, fragmentSkyBoxShader);
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

void Renderer::textureUniformDefault(std::shared_ptr<ShaderProgram>& shaderProgram, std::shared_ptr<Polytope>& polytope) {
    unsigned int index = 0;
    std::vector<std::shared_ptr<Texture>> textures = polytope->getTextures();
    if(!textures.empty()) {
        for(auto& texture : textures) {
            if(texture->getType() == Texture::Type::TextureDiffuse) {
                texture->bind();
                shaderProgram->uniformInt("tex", texture->getID() - 1);
                shaderProgram->uniformInt("hasTexture", true);
                index ++;
            }
        }
    }else shaderProgram->uniformInt("hasTexture", false);
}

void Renderer::textureUniformLighting(std::shared_ptr<ShaderProgram>& shaderProgram, std::shared_ptr<Polytope>& polytope) {
    unsigned int nDiffuseMaps = 0, nSpecularMaps = 0, nEmissionMap = 0, nNormalMaps = 0, nHeightMaps = 0;
    for(auto& texture : polytope->getTextures()) {
        texture->bind();
        switch(texture->getType()) {
            case Texture::Type::TextureDiffuse:
                shaderProgram->uniformInt("materialMaps.diffuseMap", texture->getID() - 1);
                nDiffuseMaps ++;
            break;
            case Texture::Type::TextureSpecular:
                shaderProgram->uniformInt("materialMaps.specularMap", texture->getID() - 1);
                nSpecularMaps ++;
            break;
            case Texture::Type::TextureEmission:
                shaderProgram->uniformInt("materialMaps.emissionMap", texture->getID() - 1);
                nEmissionMap ++;
            break;
            case Texture::Type::TextureNormal:

                nNormalMaps ++;
            break;
            case Texture::Type::TextureHeight:

                nHeightMaps ++;
            break;
        }
    }
    shaderProgram->uniformInt("hasDiffuse", nDiffuseMaps > 0);
    shaderProgram->uniformInt("hasSpecular", nSpecularMaps > 0);
    shaderProgram->uniformInt("hasEmission", nEmissionMap > 0);
}

void Renderer::textureUniform(std::shared_ptr<ShaderProgram>& shaderProgram, std::shared_ptr<Polytope>& polytope, bool hasLight) {
    if(!hasLight) textureUniformDefault(shaderProgram, polytope);
    else textureUniformLighting(shaderProgram, polytope);
}

void Renderer::primitiveSettings(Group* group) {
    glPointSize(group->getPointSize());
    glLineWidth(group->getLineWidth());
}

void Renderer::defaultPrimitiveSettings() {
    glPointSize(1.0f);
    glLineWidth(1.0f);
}

void Renderer::lightShaderUniforms() {
    shaderProgramLighting->uniformInt("nLights", nLights);
    for(int i = 0; i < nLights; i ++) {
        std::string lightUniform = "lights[" + std::to_string(i) + "]";
        shaderProgramLighting->uniformVec3(lightUniform + ".position", lights[i]->getPosition());
        shaderProgramLighting->uniformVec3(lightUniform + ".color", lights[i]->getColor());
        shaderProgramLighting->uniformVec3(lightUniform + ".ambient", lights[i]->getAmbient());
        shaderProgramLighting->uniformVec3(lightUniform + ".diffuse", lights[i]->getDiffuse());
        shaderProgramLighting->uniformVec3(lightUniform + ".specular", lights[i]->getSpecular());
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
            textureUniform(shaderProgram, polytope, false);
        } 
        else if(hasCamera && hasLight) {
            lightShaderUniforms();
            lightMaterialUniforms(polytope);
            textureUniform(shaderProgramLighting, polytope, true);
            lightMVPuniform(model);
        }
        // Set face culling
        switch(polytope->getFaceCulling()) {
            case Polytope::FaceCulling::FRONT:
                enableFrontFaceCulling();
            break;
            case Polytope::FaceCulling::BACK:
                enableBackFaceCulling();
            break;
            case Polytope::FaceCulling::NONE:
                disableFaceCulling();
            break;
        }
        // Draw polytope
        polytope->draw(group->getPrimitive(), group->isShowWire());
        if(polytope->isSelected()) {
            shaderProgramSelection->useProgram();
            shaderProgramSelection->uniformMat4("mvp", mvp);

            glDisable(GL_DEPTH_TEST);
            polytope->draw(group->getPrimitive(), group->isShowWire());
            glEnable(GL_DEPTH_TEST);

        }
        // unbind textures
        for(auto& texture : polytope->getTextures()) texture->unbind();
    }

    // Set default primitive settings
    defaultPrimitiveSettings();
}

void Renderer::drawSkyBox() {
    if(skyBox == nullptr) return;
    shaderProgramSkyBox->useProgram();
    // remove translation from the view matrix
    view = glm::mat4(glm::mat3(camera->getViewMatrix())); 
    shaderProgramSkyBox->uniformInt("skybox", 0);
    shaderProgramSkyBox->uniformMat4("view", view);
    shaderProgramSkyBox->uniformMat4("projection", projection);
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
    // Draw skybox
    drawSkyBox();
    // Init transform matrices
    if(hasCamera) {
        projection = camera->getProjectionMatrix();
        view = camera->getViewMatrix();
    }
    // Draw groups
    for(Group* group : groups) drawGroup(group);
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
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
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

void Renderer::disableFaceCulling() {
    glDisable(GL_CULL_FACE);
}