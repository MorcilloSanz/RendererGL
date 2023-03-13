#include "Renderer.h"

#include "../../../glew/glew.h"

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
   return dynamic_cast<const Base*>(ptr) != nullptr;
}

Renderer::Renderer(unsigned int _viewportWidth, unsigned int _viewportHeight) 
    : camera(nullptr), hasCamera(false), hasLight(false), nLights(0),
    projection(glm::mat4(1.f)), view(glm::mat4(1.f)), depthMapFBO(0),
    depthMap(0), viewportWidth(_viewportWidth), viewportHeight(_viewportHeight),
    shadowLightPos(0, 0, 0), shadowMapping(false) {
    initShaders();
    enableBlending();
    enableAntialiasing();
    initShadowMapping();
}

Renderer::Renderer() 
    : Renderer(0, 0) {
}

void Renderer::initShaders() {
    // Default shader program
    Shader vertexShader = Shader::fromFile("glsl/Default.vert", Shader::ShaderType::Vertex);
    Shader fragmentShader = Shader::fromFile("glsl/Default.frag", Shader::ShaderType::Fragment);
    shaderProgram = ShaderProgram::New(vertexShader, fragmentShader);
    // Lighting shader program
    Shader vertexLightingShader = Shader::fromFile("glsl/BlinnPhong.vert", Shader::ShaderType::Vertex);
    Shader fragmentLightingShader = Shader::fromFile("glsl/BlinnPhong.frag", Shader::ShaderType::Fragment);
    shaderProgramLighting = ShaderProgram::New(vertexLightingShader, fragmentLightingShader);
    // Depth Map shader program
    Shader vertexDepthMapShader = Shader::fromFile("glsl/SimpleDepth.vert", Shader::ShaderType::Vertex);
    Shader fragmentDepthMapShader = Shader::fromFile("glsl/SimpleDepth.frag", Shader::ShaderType::Fragment);
    shaderProgramDepthMap = ShaderProgram::New(vertexDepthMapShader, fragmentDepthMapShader);
    // SkyBox shader program
    Shader vertexSkyBoxShader = Shader::fromFile("glsl/SkyBox.vert", Shader::ShaderType::Vertex);
    Shader fragmentSkyBoxShader = Shader::fromFile("glsl/SkyBox.frag", Shader::ShaderType::Fragment);
    shaderProgramSkyBox = ShaderProgram::New(vertexSkyBoxShader, fragmentSkyBoxShader);
    // Selection shader program
    Shader vertexSelectionShader = Shader::fromFile("glsl/Selection.vert", Shader::ShaderType::Vertex);
    Shader fragmentSelectionShader = Shader::fromFile("glsl/Selection.frag", Shader::ShaderType::Fragment);
    shaderProgramSelection = ShaderProgram::New(vertexSelectionShader, fragmentSelectionShader);
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

void Renderer::removeLight(Light& light) {
    unsigned int index = 0;
    for(Light* l : lights) {
        if(l == &light) {
            removeLight(index);
            break;
        }
        index ++;
    }
}

void Renderer::textureUniformDefault(ShaderProgram::Ptr& shaderProgram, std::shared_ptr<Polytope>& polytope) {
    unsigned int index = 0;
    std::vector<Texture::Ptr> textures = polytope->getTextures();
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

void Renderer::textureUniformLighting(ShaderProgram::Ptr& shaderProgram, std::shared_ptr<Polytope>& polytope) {
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

void Renderer::textureUniform(ShaderProgram::Ptr& shaderProgram, std::shared_ptr<Polytope>& polytope, bool hasLight) {
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
        // Directional Light
        std::string lightUniform = "lights[" + std::to_string(i) + "]";
        shaderProgramLighting->uniformVec3(lightUniform + ".position", lights[i]->getPosition());
        shaderProgramLighting->uniformVec3(lightUniform + ".color", lights[i]->getColor());
        shaderProgramLighting->uniformVec3(lightUniform + ".ambient", lights[i]->getAmbient());
        shaderProgramLighting->uniformVec3(lightUniform + ".diffuse", lights[i]->getDiffuse());
        shaderProgramLighting->uniformVec3(lightUniform + ".specular", lights[i]->getSpecular());
        // Point Light (Spot Light is also a Point Light)
        if(instanceof<PointLight>(lights[i])) {
            PointLight* pointLight = dynamic_cast<PointLight*>(lights[i]);
            shaderProgramLighting->uniformInt("isPointLight", true);
            shaderProgramLighting->uniformFloat(lightUniform + ".constant", pointLight->getConstant());
            shaderProgramLighting->uniformFloat(lightUniform + ".linear", pointLight->getLinear());
            shaderProgramLighting->uniformFloat(lightUniform + ".quadratic", pointLight->getQuadratic());
        }else shaderProgramLighting->uniformInt("isPointLight", false);
        // Spot Light
        if(instanceof<SpotLight>(lights[i])) {
            SpotLight* spotLight = dynamic_cast<SpotLight*>(lights[i]);
            shaderProgramLighting->uniformInt("isSpotLight", true);
            shaderProgramLighting->uniformVec3(lightUniform + ".direction", spotLight->getDirection());
            shaderProgramLighting->uniformFloat(lightUniform + ".cutOff", spotLight->getCutOff());
            shaderProgramLighting->uniformFloat(lightUniform + ".outerCutOff", spotLight->getOuterCutOff());
        }else shaderProgramLighting->uniformInt("isSpotLight", false);
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

void Renderer::shadowMappingUniforms() {
    if(!shadowMapping) return;
    depthMap->bind();
    shaderProgramLighting->uniformInt("shadowMap", depthMap->getID() - 1);
    shaderProgramLighting->uniformMat4("lightSpaceMatrix", lightSpaceMatrix);
    shaderProgramLighting->uniformVec3("lightPos", shadowLightPos);
}

void Renderer::setFaceCulling(const Polytope::Ptr& polytope) {
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
}

void Renderer::setViewport(unsigned int viewportWidth, unsigned int viewportHeight) {
    this->viewportWidth = viewportWidth;
    this->viewportHeight = viewportHeight;
}

void Renderer::initShadowMapping() {

    glGenFramebuffers(1, &depthMapFBO);  

    depthMap = DepthTexture::New(viewportWidth, viewportWidth);
    depthMap->bind();

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap->getID(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shaderProgramLighting->useProgram();
    shaderProgramLighting->uniformInt("shadowMap", depthMap->getID() - 1);
}

void Renderer::renderToDepthMap(Group* group) {
    if(!hasLight || !group->isVisible()) return;

    int previousFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFBO);

    glViewport(0, 0, viewportWidth, viewportWidth);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    // Shaders
    float nearPlane = 1.f, farPlane = 17.5f;
    glm::mat4 lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, nearPlane, farPlane);
    glm::mat4 lightView = glm::lookAt(shadowLightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;

    shaderProgramDepthMap->useProgram();
    shaderProgramDepthMap->uniformMat4("lightSpaceMatrix", lightSpaceMatrix);

    // Draw
    glClear(GL_DEPTH_BUFFER_BIT);

    for(auto& polytope : group->getPolytopes()) {

        glm::mat4 model = group->getModelMatrix() * polytope->getModelMatrix();
        shaderProgramDepthMap->uniformMat4("model", model);

        //glCullFace(GL_FRONT);
        polytope->draw(group->getPrimitive(), group->isShowWire());
        //glCullFace(GL_BACK);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, previousFBO);
}

void Renderer::drawGroup(Group* group) {
    if(!group->isVisible()) return;

    glViewport(0, 0, viewportWidth, viewportHeight);

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
            shadowMappingUniforms();
        }
        // Set face culling
        setFaceCulling(polytope);
        // Draw polytope
        polytope->draw(group->getPrimitive(), group->isShowWire());
        // Draw selected polytope if selected
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
    for(Group* group : groups) {
        if(shadowMapping) renderToDepthMap(group);
        drawGroup(group);
    }
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