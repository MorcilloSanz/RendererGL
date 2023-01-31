#pragma once

#include "Buffer.h"

class VertexArray : public Buffer {
    GENERATE_PTR(VertexArray)
public:
    VertexArray();
    VertexArray(const VertexArray& vertexArray);
    VertexArray(VertexArray&& vertexArray) noexcept;
    VertexArray& operator=(const VertexArray& vertexArray);
    ~VertexArray();
protected:
    void initBuffer() override;
public:
    void bind() override;
    void unbind() override;
};