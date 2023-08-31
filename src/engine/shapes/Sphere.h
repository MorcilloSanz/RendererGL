#pragma once

#include "Shape.h"

class Sphere : public Shape {
    GENERATE_PTR(Sphere)
public:
    Sphere();
    Sphere(float radius, int latitudes, int longitudes);
    virtual ~Sphere() = default;
private:
    Polytope::Ptr createSphere(float radius, int latitudes, int longitudes);
};