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

#include "SkyBox.h"

class Renderer {
private:
    ShaderProgram::Ptr shaderProgram;
    ShaderProgram::Ptr shaderProgramLighting;
    ShaderProgram::Ptr shaderProgramSkyBox;
    ShaderProgram::Ptr shaderProgramSelection;
    glm::mat4 projection;
    glm::mat4 view;
    std::vector<Group*> groups;
    Camera* camera;
    bool hasCamera;
    std::vector<Light*> lights;
    unsigned int nLights;
    bool hasLight;
    glm::vec3 backgroundColor;
    SkyBox::Ptr skyBox;
public:
    Renderer();
    ~Renderer() = default;
private:
    void initShaders();
    void textureUniformDefault(ShaderProgram::Ptr& shaderProgram, Polytope::Ptr& polytope);
    void textureUniformLighting(ShaderProgram::Ptr& shaderProgram, Polytope::Ptr& polytope);
    void textureUniform(ShaderProgram::Ptr& shaderProgram, Polytope::Ptr& polytope, bool hasLight);
    void primitiveSettings(Group* group);
    void defaultPrimitiveSettings();
    void lightShaderUniforms();
    void lightMaterialUniforms(const Polytope::Ptr& polytope);
    void lightMVPuniform(const glm::mat4& model);
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

    inline ShaderProgram::Ptr& getShaderProgram() { return shaderProgram; }

    inline glm::vec3& getBackgroundColor() { return backgroundColor; }

    inline void setSkyBox(const SkyBox::Ptr& skyBox) { this->skyBox = skyBox; }

};