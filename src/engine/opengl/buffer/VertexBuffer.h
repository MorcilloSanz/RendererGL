#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Buffer.h"
#include "IndexBuffer.h"

#include "engine/Vec3.h"

class VertexBuffer : public Buffer {
    GENERATE_PTR(VertexBuffer)
private:
    IndexBuffer::Ptr indexBuffer;
    bool hasIndexBuffer;
    size_t length;
public:
    VertexBuffer();
    VertexBuffer(size_t _length);
    VertexBuffer(std::vector<Vec3f>& vertices);
    VertexBuffer(std::vector<Vec3f>& vertices, std::vector<unsigned int> indices);
    VertexBuffer(const VertexBuffer& vertexBuffer);
    VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;
    VertexBuffer& operator=(const VertexBuffer& vertexBuffer);
    ~VertexBuffer();
protected:
    void vertexAttributes();
    void initBuffer(std::vector<Vec3f>& vertices, std::vector<unsigned int> indices);
    void initBuffer() override;
public:
    void updateVertices(std::vector<Vec3f>& vertices);
    void updateVertex(int pos, Vec3f newVertex);
    std::vector<Vec3f> getVertices();
    void bind() override;
    void unbind() override;
public:
    inline IndexBuffer::Ptr& getIndexBuffer() { return indexBuffer; }
    inline bool HasIndexBuffer() const { return hasIndexBuffer; }
    inline void setLength(size_t length) { this->length = length; }
    inline size_t getLength() const { return length; }
};