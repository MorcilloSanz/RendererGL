#pragma once

#include <iostream>
#include <vector>

#include "../group/Group.h"
#include "../opengl/shader/Shader.h"

#include "Camera.h"

#include "../lighting/Light.h"
#include "../lighting/DirectionalLight.h"
#include "../lighting/PointLight.h"
#include "../lighting/SpotLight.h"

#include "../texture/DepthTexture.h"
#include "../texture/ColorBufferTexture.h"

#include "SkyBox.h"

class Renderer {
    GENERATE_PTR(Renderer)
private:
    // Shaders
    ShaderProgram::Ptr shaderProgram;
    ShaderProgram::Ptr shaderProgramLighting;
    ShaderProgram::Ptr shaderProgramDepthMap;
    ShaderProgram::Ptr shaderProgramHDR;
    ShaderProgram::Ptr shaderProgramSkyBox;
    ShaderProgram::Ptr shaderProgramSelection;

    // Group visualization
    glm::mat4 projection;
    glm::mat4 view;

    std::vector<Group*> groups;

    // Camera
    Camera* camera;
    bool hasCamera;

    // Lighting
    std::vector<Light*> lights;
    unsigned int nLights;
    bool hasLight;

    // Shadow Mapping
    unsigned int depthMapFBO;
    DepthTexture::Ptr depthMap;

    glm::mat4 lightSpaceMatrix;
    glm::vec3 shadowLightPos;
    bool shadowMapping;

    // HDR
    unsigned int hdrFBO;
    ColorBufferTexture::Ptr colorBufferTexture;
    unsigned int rboDepth;

    bool hdr;
    float exposure;
    bool gammaCorrection;

    // Texture Quad
    VertexArray::Ptr quadVAO;
    VertexBuffer::Ptr quadVBO;

    // Background and skybox
    glm::vec3 backgroundColor;
    SkyBox::Ptr skyBox;

    unsigned int viewportWidth, viewportHeight;
public:
    Renderer(unsigned int _viewportWidth, unsigned int _viewportHeight);
    Renderer();
    ~Renderer() = default;
private:
    void initShaders();
    void initTextureQuad();
    void textureUniformDefault(ShaderProgram::Ptr& shaderProgram, Polytope::Ptr& polytope);
    void textureUniformLighting(ShaderProgram::Ptr& shaderProgram, Polytope::Ptr& polytope);
    void textureUniform(ShaderProgram::Ptr& shaderProgram, Polytope::Ptr& polytope, bool hasLight);
    void initShadowMapping();
    void initHDR();
    void primitiveSettings(Group* group);
    void defaultPrimitiveSettings();
    void lightShaderUniforms();
    void lightMaterialUniforms(const Polytope::Ptr& polytope);
    void lightMVPuniform(const glm::mat4& model);
    void shadowMappingUniforms();
    void renderToDepthMap();
    void renderQuad();
    void drawGroup(Group* group);
    void drawSkyBox();
public:
    void removeGroup(Group& group);
    void removeLight(Light& light);
    void render();
    void setBackgroundColor(float r, float g, float b);
    void setCamera(Camera& camera);
    void setLight(Light& light);
    void addLight(Light& light);
    void clear();
    void enableBlending();
    void enableAntialiasing();
    void enableBackFaceCulling();   // Counter-clockwise order
    void enableFrontFaceCulling();  // Counter-clockwise order
    void disableFaceCulling();
    void setFaceCulling(const Polytope::Ptr& polytope);
    void setViewport(unsigned int viewportWidth, unsigned int viewportHeight);
public:
    inline void addGroup(Group& group) { groups.push_back(&group); }
    inline void removeGroup(int index) { groups.erase(groups.begin() + index); }
    inline Group* getGroup(int index) { return groups[index]; }
    inline std::vector<Group*>& getGroups() { return groups; }

    inline Camera* getCamera() { return camera; }

    inline void enableLight() { hasLight = true; }
    inline void disableLight() { hasLight = false; } 
    inline void setLightEnabled(bool enable) { hasLight = enable; }
    inline void removeLight(int index) { lights.erase(lights.begin() + index); nLights --; }
    inline Light* getLight(int index) { return lights[index]; }
    inline std::vector<Light*>& getLights() { return lights; }

    inline void setShadowLightPos(const glm::vec3& shadowLightPos) { this->shadowLightPos = shadowLightPos; }
    inline glm::vec3& getShadowLightPos() { return shadowLightPos; }

    inline void setShadowMapping(bool shadowMapping) { this->shadowMapping = shadowMapping; }
    inline bool isShadowMapping() const { return shadowMapping; }

    inline ShaderProgram::Ptr& getShaderProgram() { return shaderProgram; }

    inline glm::vec3& getBackgroundColor() { return backgroundColor; }

    inline void setSkyBox(const SkyBox::Ptr& skyBox) { this->skyBox = skyBox; }

    inline void setViewportWidth(unsigned int viewportWidth) { this->viewportWidth = viewportWidth; }
    inline unsigned int getViewportWidth() const { return viewportWidth; }

    inline void setViewportHeight(unsigned int viewportHeight) { this->viewportHeight = viewportHeight; }
    inline unsigned int getViewportHeight() const { return viewportHeight; }

    inline DepthTexture::Ptr& getDepthMap() { return depthMap; }

    inline void setHDR(bool hdr) { this->hdr = hdr; }
    inline bool isHDR() const { return hdr; }

    inline void setExposure(float exposure) { this->exposure = exposure; }
    inline float getExposure() const { return exposure; }

    inline void setGammaCorrection(bool gammaCorrection) { this->gammaCorrection = gammaCorrection; }
    inline bool isGammaCorrection() const { return gammaCorrection; }
};