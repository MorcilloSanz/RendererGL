#pragma once

#include "Buffer.h"

class RenderBuffer : public Buffer {
    GENERATE_PTR(RenderBuffer)
protected:
    unsigned int width, height;
    int internalFormat;
public:
    RenderBuffer(unsigned int _width, unsigned int _height, int _internalFormat = GL_DEPTH_COMPONENT);
    RenderBuffer() = default;
    virtual ~RenderBuffer();
protected:
    void initBuffer() override;
public:
    void bind() override;
    void unbind() override;
public:
    inline unsigned int getWidth() const { return width; }
    inline unsigned int getHeight() const { return height; }
    inline int getInternalFormat() const { return internalFormat; }
};