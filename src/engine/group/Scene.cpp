#include "Scene.h"

Scene::Scene() 
    : modelMatrix(1.f), visible(true) {    
}

void Scene::removeGroup(Group::Ptr& group) {
    unsigned int index = 0;
    for(auto& g : groups) {
        if(g.get() == group.get()) {
            removeGroup(index);
            break;
        }
        index ++;
    }
}

void Scene::removeScene(Scene::Ptr& scene) {
    unsigned int index = 0;
    for(auto& s : scenes) {
        if(s.get() == scene.get()) {
            removeScene(index);
            break;
        }
        index ++;
    }
}

void Scene::addModel(Model::Ptr& model) {
    Group* group = dynamic_cast<Group*>(model.get());
    Group::Ptr groupPtr = Group::New(*group);
    groups.push_back(groupPtr);
}

void Scene::removeModel(Model::Ptr& model) {
    
    Group* group = dynamic_cast<Group*>(model.get());
    
    int index = 0;
    for(auto& g : groups) {
        if(g->getID() == group->getID()) {
            removeGroup(index);
            break;
        }
        index ++;
    }
}

void Scene::translate(const glm::vec3& v) { 
    modelMatrix = glm::translate(modelMatrix, v); 
    for(auto& childScene : scenes) childScene->translate(v);
}

void Scene::rotate(float degrees, const glm::vec3& axis) { 
    modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), axis); 
    for(auto& childScene : scenes) childScene->rotate(degrees, axis);
}

void Scene::scale(const glm::vec3& s) { 
    modelMatrix = glm::scale(modelMatrix, s); 
    for(auto& childScene : scenes) childScene->scale(s);
}