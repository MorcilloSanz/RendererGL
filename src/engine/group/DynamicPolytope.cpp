#include "DynamicPolytope.h"

DynamicPolytope::DynamicPolytope(size_t length) 
    : Polytope(length), pos(0) {
}

void DynamicPolytope::addVertex(const Vec3f& vertex) {
    for(int i = pos; i < vertexLength; i ++) updateVertex(i, vertex);
    pos ++;
}