#pragma once

#include "Material.h"

class PhongMaterial : public Material {
    GENERATE_PTR(PhongMaterial)
private:
    glm::vec3 diffuse, specular;
    float shininess;
public:
    PhongMaterial(const glm::vec3& _diffuse, const glm::vec3& _specular, float _shininess)
        : Material(Material::MaterialType::Phong), diffuse(_diffuse), specular(_specular), 
        shininess(_shininess) {  
    }

    PhongMaterial() = default;
    ~PhongMaterial() = default;
public:
    inline void setDiffuse(const glm::vec3& diffuse) { this->diffuse = diffuse; }
    inline glm::vec3& getDiffuse() { return diffuse; }

    inline void setSpecular(const glm::vec3& specular) { this->specular = specular; }
    inline glm::vec3& getSpecular() { return specular; }

    /**
     * @brief shininess: 2, 4, 8, 16, 32, 64, ..., 256
     * 
     * @param shininess 
     */
    inline void setShininess(const float shininess) { this->shininess = shininess; }
    inline float getShininess() const { return shininess; }
};