#pragma once

#include "Material.h"

class PBRMaterial : public Material {
    GENERATE_PTR(PBRMaterial)
public:
    PBRMaterial() 
        : Material(Material::MaterialType::PBR) {
    }
};