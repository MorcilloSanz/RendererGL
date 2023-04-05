#pragma once

#include "Buffer.h"

class MultiSampleRenderBuffer : public Buffer {
    GENERATE_PTR(MultiSampleRenderBuffer)
protected:
    unsigned int width, height;
    unsigned int samples;
public:
    MultiSampleRenderBuffer(unsigned int _width, unsigned int _height, unsigned int _samples = 4);
    MultiSampleRenderBuffer() = default;
    virtual ~MultiSampleRenderBuffer();
protected:
    void initBuffer() override;
public:
    void bind() override;
    void unbind() override;
public:
    inline unsigned int getWidth() const { return width; }
    inline unsigned int getHeight() const { return height; }
    inline unsigned int getSamples() const { return samples; }
};