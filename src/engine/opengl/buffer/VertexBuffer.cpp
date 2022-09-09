#include "VertexBuffer.h"

#include <string.h>

VertexBuffer::VertexBuffer() : Buffer() { }

VertexBuffer::VertexBuffer(const std::vector<Vec3f>& _vertices)
    : Buffer(), vertices(_vertices), hasIndexBuffer(false) {
    initBuffer();
}

VertexBuffer::VertexBuffer(const std::vector<Vec3f>& _vertices, const std::vector<unsigned int> _indices) 
    : Buffer(), vertices(_vertices), hasIndexBuffer(true), indices(_indices) {
    initBuffer();
}

VertexBuffer::VertexBuffer(const VertexBuffer& vertexBuffer) 
    : vertices(vertexBuffer.vertices), hasIndexBuffer(vertexBuffer.hasIndexBuffer) {
    id = vertexBuffer.id;
    if(vertexBuffer.indexBuffer != nullptr)
        indexBuffer = vertexBuffer.indexBuffer;
}

VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept 
    : vertices(std::move(vertexBuffer.vertices)), hasIndexBuffer(vertexBuffer.hasIndexBuffer) {
    id = vertexBuffer.id;
    if(indexBuffer != nullptr)
        indexBuffer = std::move(vertexBuffer.indexBuffer);
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& vertexBuffer) {
    vertices = vertexBuffer.vertices;
    id = vertexBuffer.id;
    hasIndexBuffer = vertexBuffer.hasIndexBuffer;
    if(indexBuffer != nullptr)
        indexBuffer = vertexBuffer.indexBuffer;
    return *this;
}

VertexBuffer::~VertexBuffer() {
    unbind();
    glDeleteBuffers(1, &id);
}

void VertexBuffer::initBuffer() {
    // Load vertices
    unsigned int index = 0;
    float* glVertices = new float[vertices.size() * 11];
    for(Vec3f& vertex : vertices) {
        glVertices[index] = vertex.x;   glVertices[index + 1] = vertex.y;   glVertices[index + 2] = vertex.z;
        glVertices[index + 3] = vertex.r;   glVertices[index + 4] = vertex.g;   glVertices[index + 5] = vertex.b;
        glVertices[index + 6] = vertex.nx;   glVertices[index + 7] = vertex.ny;   glVertices[index + 8] = vertex.nz;
        glVertices[index + 9] = vertex.tx;  glVertices[index + 10] = vertex.ty;
        index += 11;
    }
    // Vertex buffer
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 11, glVertices, GL_DYNAMIC_DRAW);
    delete[] glVertices;
    // Index Buffer
    if(hasIndexBuffer) indexBuffer = std::make_shared<IndexBuffer>(indices);
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    // normal attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    // texture coordinates attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
}

void VertexBuffer::updateVertices(std::vector<Vec3f>& vertices, bool copy2memory) {
    if(copy2memory) this->vertices = vertices;
    glBindBuffer(GL_ARRAY_BUFFER, id);
    float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    unsigned int index = 0;
    for(Vec3f& vertex : vertices) {
        ptr[index] = vertex.x;     ptr[index + 1] = vertex.y; ptr[index + 2] = vertex.z;
        ptr[index + 3] = vertex.r; ptr[index + 4] = vertex.g; ptr[index + 5] = vertex.b;
        ptr[index + 6] = vertex.nx; ptr[index + 7] = vertex.ny; ptr[index + 8] = vertex.nz;
        ptr[index + 9] = vertex.tx; ptr[index + 10] = vertex.ty;
        index += 11;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}