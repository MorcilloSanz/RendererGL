#pragma once

#include "../../../../glew/glew.h"

class Buffer {
protected:
    unsigned int id;
    virtual void initBuffer() = 0;
public:
    Buffer() : id(0) {}
    virtual void bind() = 0;
    virtual void unbind() = 0;
public:
    inline unsigned int getID() const { return id; }
};