#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Buffer.h"
#include "IndexBuffer.h"

#include "../../Vec3.h"

class VertexBuffer : public Buffer {
private:
    std::vector<Vec3f> vertices;
    std::vector<unsigned int> indices;
    std::shared_ptr<IndexBuffer> indexBuffer;
    bool hasIndexBuffer;
public:
    VertexBuffer();
    VertexBuffer(const std::vector<Vec3f>& _vertices);
    VertexBuffer(const std::vector<Vec3f>& _vertices, const std::vector<unsigned int> _indices);
    VertexBuffer(const VertexBuffer& vertexBuffer);
    VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;
    VertexBuffer& operator=(const VertexBuffer& vertexBuffer);
    ~VertexBuffer();
protected:
    void initBuffer() override;
public:
    void updateVertices(std::vector<Vec3f>& vertices, bool copy2memory = false);
    void bind() override;
    void unbind() override;
public:
    inline std::vector<Vec3f>& getVertices() {
        return vertices;
    }

    inline float* getMapBuffer() {
        return (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    }

    inline std::shared_ptr<IndexBuffer>& getIndexBuffer() {
        return indexBuffer;
    }

    inline bool HasIndexBuffer() const {
        return hasIndexBuffer;
    }
};