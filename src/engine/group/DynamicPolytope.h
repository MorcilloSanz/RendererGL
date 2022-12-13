#pragma once

#include "Polytope.h"

/*
    TODO: Resize if needed
*/
class DynamicPolytope : public Polytope {
private:
    size_t pos;
public:
    DynamicPolytope() = default;
    DynamicPolytope(size_t length);
public:
    void addVertex(const Vec3f& vertex);
};