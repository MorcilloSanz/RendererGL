#pragma once

#include "Shape.h"

class Cube : public Shape {
    GENERATE_PTR(Cube)
public:
    Cube();
    virtual ~Cube() = default;
};