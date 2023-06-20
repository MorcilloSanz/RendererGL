#pragma once

#include "Material.h"

class PBRMaterial : public Material {
    GENERATE_PTR(PBRMaterial)
private:
    glm::vec3 albedo;
    float metallic;
    float roughness;
    float ao;
public:
    
    PBRMaterial(const glm::vec3& _albedo, float _metallic, float _roughness, float _ao)
        : Material(Material::MaterialType::PBR), albedo(_albedo), metallic(_metallic), 
        roughness(_roughness), ao(_ao) {
    }
    
    PBRMaterial() 
        : Material(Material::MaterialType::PBR) {
    }

    ~PBRMaterial() = default;
public:

    inline void setAlbedo(const glm::vec3& albedo) { this->albedo = albedo; }
    inline glm::vec3& getAlbedo() { return albedo; }

    inline void setMetallic(float metallic) { this->metallic = metallic; }
    inline float getMetallic() const { return metallic; }

    inline void setRoughness(float roughness) { this->roughness = roughness; }
    inline float getRoughness() const { return roughness; }

    inline void setAmbientOcclusion(float ao) { this->ao = ao; }
    inline float getAmbientOcclusion() const { return ao; }
};