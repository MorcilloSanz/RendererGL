#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "../group/Group.h"
#include "../opengl/shader/Shader.h"

#include "Camera.h"

#include "../lighting/Light.h"
#include "../lighting/DirectionalLight.h"
#include "../lighting/PointLight.h"

#include "SkyBox.h"

class Renderer {
private:
    std::shared_ptr<ShaderProgram> shaderProgram;
    std::shared_ptr<ShaderProgram> shaderProgramLighting;
    std::shared_ptr<ShaderProgram> shaderProgramSkyBox;
    std::shared_ptr<ShaderProgram> shaderProgramSelection;
    glm::mat4 projection;
    glm::mat4 view;
    std::vector<Group*> groups;
    Camera* camera;
    bool hasCamera;
    std::vector<Light*> lights;
    unsigned int nLights;
    bool hasLight;
    glm::vec3 backgroundColor;
    std::shared_ptr<SkyBox> skyBox;
public:
    Renderer();
    ~Renderer() = default;
private:
    void initShaders();
    void textureUniformDefault(std::shared_ptr<ShaderProgram>& shaderProgram, std::shared_ptr<Polytope>& polytope);
    void textureUniformLighting(std::shared_ptr<ShaderProgram>& shaderProgram, std::shared_ptr<Polytope>& polytope);
    void textureUniform(std::shared_ptr<ShaderProgram>& shaderProgram, std::shared_ptr<Polytope>& polytope, bool hasLight);
    void primitiveSettings(Group* group);
    void defaultPrimitiveSettings();
    void lightShaderUniforms();
    void lightMaterialUniforms(const std::shared_ptr<Polytope>& polytope);
    void lightMVPuniform(const glm::mat4& model);
    void drawGroup(Group* group);
    void drawSkyBox();
public:
    void removeGroup(Group& group);
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
public:
    inline void addGroup(Group& group) { groups.push_back(&group); }
    inline void removeGroup(int index) { groups.erase(groups.begin() + index); }
    inline Group* getGroup(int index) { return groups[index]; }
    inline std::vector<Group*>& getGroups() { return groups; }

    inline Camera* getCamera() { return camera; }

    inline void enableLight() { hasLight = true; }
    inline void disableLight() { hasLight = false; } 
    inline void setLightEnabled(bool enable) { hasLight = enable; }

    inline std::shared_ptr<ShaderProgram>& getShaderProgram() { return shaderProgram; }

    inline glm::vec3& getBackgroundColor() { return backgroundColor; }

    inline void setSkyBox(const std::shared_ptr<SkyBox>& skyBox) { this->skyBox = skyBox; }

    inline std::vector<Light*>& getLights() { return lights; }
};