#pragma once

#include "RenderBuffer.h"

class MultiSampleRenderBuffer : public RenderBuffer {
    GENERATE_PTR(MultiSampleRenderBuffer)
protected:
    unsigned int samples;
public:
    MultiSampleRenderBuffer(unsigned int width, unsigned int height, unsigned int _samples = 4);
    MultiSampleRenderBuffer() = default;
    virtual ~MultiSampleRenderBuffer() = default;
protected:
    void initBuffer() override;
public:
    inline unsigned int getSamples() const { return samples; }
};