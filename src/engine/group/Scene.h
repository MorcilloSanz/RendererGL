#pragma once

#include "Group.h"
#include "engine/model/Model.h"

#include "engine/ptr.h"

class Scene {
    GENERATE_PTR(Scene)
private:
    std::vector<Group::Ptr> groups;
    std::vector<Scene::Ptr> scenes;
    glm::mat4 modelMatrix;
    bool visible;
public:
    Scene();
    ~Scene() = default;
public:
    void removeGroup(Group::Ptr& group);
    void removeScene(Scene::Ptr& scene);

    void addModel(Model::Ptr& model);
    void removeModel(Model::Ptr& model);

    void translate(const glm::vec3& v);
    void rotate(float degrees, const glm::vec3& axis);
    void scale(const glm::vec3& s);
public:
    inline void addGroup(Group::Ptr& group) { groups.push_back(group); }
    inline void addScene(Scene::Ptr& scene) { scenes.push_back(scene); }

    inline void removeGroup(int index) { groups.erase(groups.begin() + index); }
    inline void removeScene(int index) { scenes.erase(scenes.begin() + index); }

    inline std::vector<Group::Ptr>& getGroups() { return groups; }
    inline std::vector<Scene::Ptr>& getScenes() { return scenes; }

    inline void setModelMatrix(const glm::mat4& modelMatrix) { this->modelMatrix = modelMatrix; }
    inline glm::mat4& getModelMatrix() { return modelMatrix; }

    inline void setVisible(bool visible) { this->visible = visible; }
    inline bool isVisible() const { return visible; }
};