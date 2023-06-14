#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/ptr.h"

class Material {
    GENERATE_PTR(Material)
public:
    enum class MaterialType {
        None, Phong, PBR
    };
protected:
    MaterialType materialType;
public:
    Material(MaterialType _materialType) 
        : materialType(_materialType) {
    }
    Material() = default;
    virtual ~Material() = default;
public:
    inline MaterialType& getMaterialType() { return materialType; }
};