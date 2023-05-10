#pragma once

#include <GL/glew.h>

#include "../../ptr.h"

class Buffer {
protected:
    unsigned int id;
    virtual void initBuffer() = 0;
public:
    Buffer() : id(0) {}
    virtual ~Buffer() = default;
public:
    virtual void bind() = 0;
    virtual void unbind() = 0;
public:
    inline unsigned int getID() const { return id; }
};