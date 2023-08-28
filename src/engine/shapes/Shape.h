#pragma once

#include "engine/group/Polytope.h"

class Shape : public Polytope {
    GENERATE_PTR(Shape)
public:
    Shape(size_t length);
    Shape(std::vector<Vec3f>& vertices, bool tangentAndBitangents = true);
    Shape(std::vector<Vec3f>& vertices, std::vector<unsigned int>& indices, bool tangentAndBitangents = true);
    virtual ~Shape() = default;
};