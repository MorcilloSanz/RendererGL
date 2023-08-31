#pragma once

#include "engine/group/Polytope.h"

class Shape : public Polytope {
    GENERATE_PTR(Shape)
public:
    Shape() = default;
    Shape(size_t length);
    Shape(Polytope::Ptr polytope);
    Shape(std::vector<Vec3f>& vertices, bool tangentAndBitangents = true);
    Shape(std::vector<Vec3f>& vertices, std::vector<unsigned int>& indices, bool tangentAndBitangents = true);
    virtual ~Shape() = default;
};