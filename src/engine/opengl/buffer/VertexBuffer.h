#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Buffer.h"
#include "IndexBuffer.h"

#include "../../Vec3.h"

class VertexBuffer : public Buffer {
private:
    std::shared_ptr<IndexBuffer> indexBuffer;
    bool hasIndexBuffer;
public:
    VertexBuffer();
    VertexBuffer(std::vector<Vec3f>& vertices);
    VertexBuffer(std::vector<Vec3f>& vertices, std::vector<unsigned int> indices);
    VertexBuffer(const VertexBuffer& vertexBuffer);
    VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;
    VertexBuffer& operator=(const VertexBuffer& vertexBuffer);
    ~VertexBuffer();
protected:
    void initBuffer(std::vector<Vec3f>& vertices);
    void initBuffer(std::vector<Vec3f>& vertices, std::vector<unsigned int> indices);
    void initBuffer() override;
public:
    void updateVertices(std::vector<Vec3f>& vertices);
    void updateVertex(int pos, Vec3f* newVertex);
    void bind() override;
    void unbind() override;
public:
    inline float* getVerticesData() { return (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY); }
    inline std::shared_ptr<IndexBuffer>& getIndexBuffer() { return indexBuffer; }
    inline bool HasIndexBuffer() const { return hasIndexBuffer; }
};