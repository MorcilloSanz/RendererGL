#include "Shape.h"

Shape::Shape(size_t length) 
    : Polytope(length) {
}

Shape::Shape(std::vector<Vec3f>& vertices, bool tangentAndBitangents) 
    : Polytope(vertices, tangentAndBitangents) {
}

Shape::Shape(std::vector<Vec3f>& vertices, std::vector<unsigned int>& indices, bool tangentAndBitangents) 
    : Polytope(vertices, indices, tangentAndBitangents) {
}