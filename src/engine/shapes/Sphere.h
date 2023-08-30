#pragma once

#include "Shape.h"

class Sphere : public Shape {
    GENERATE_PTR(Sphere)
public:
    Sphere();
    virtual ~Sphere() = default;
};