#include "VertexBuffer.h"

#include <string.h>

VertexBuffer::VertexBuffer() : Buffer() { }

VertexBuffer::VertexBuffer(size_t _length) 
    : Buffer(), length(_length), hasIndexBuffer(false) {
    initBuffer();
}

VertexBuffer::VertexBuffer(std::vector<Vec3f>& vertices)
    : Buffer(), length(vertices.size()), hasIndexBuffer(false) {
    initBuffer(vertices);
}

VertexBuffer::VertexBuffer(std::vector<Vec3f>& vertices, std::vector<unsigned int> indices) 
    : Buffer(), length(vertices.size()), hasIndexBuffer(true) {
    initBuffer(vertices, indices);
}

VertexBuffer::VertexBuffer(const VertexBuffer& vertexBuffer) 
    : length(vertexBuffer.length), hasIndexBuffer(vertexBuffer.hasIndexBuffer) {
    if(vertexBuffer.indexBuffer != nullptr) indexBuffer = vertexBuffer.indexBuffer;
    id = vertexBuffer.id;
}

VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept 
    : length(vertexBuffer.length), hasIndexBuffer(vertexBuffer.hasIndexBuffer) {
    if(indexBuffer != nullptr) indexBuffer = std::move(vertexBuffer.indexBuffer);
    id = vertexBuffer.id;
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& vertexBuffer) {
    id = vertexBuffer.id;
    length = vertexBuffer.length;
    hasIndexBuffer = vertexBuffer.hasIndexBuffer;
    if(indexBuffer != nullptr) indexBuffer = vertexBuffer.indexBuffer;
    return *this;
}

VertexBuffer::~VertexBuffer() {
    unbind();
    glDeleteBuffers(1, &id);
}

/*
TODO: Refractor repeated code
*/
void VertexBuffer::initBuffer(std::vector<Vec3f>& vertices) {
    // Load vertices
    unsigned int index = 0;
    float* glVertices = new float[vertices.size() * 17];
    for(Vec3f& vertex : vertices) {
        glVertices[index] = vertex.x;   glVertices[index + 1] = vertex.y;   glVertices[index + 2] = vertex.z;
        glVertices[index + 3] = vertex.r;   glVertices[index + 4] = vertex.g;   glVertices[index + 5] = vertex.b;
        glVertices[index + 6] = vertex.nx;   glVertices[index + 7] = vertex.ny;   glVertices[index + 8] = vertex.nz;
        glVertices[index + 9] = vertex.tx;  glVertices[index + 10] = vertex.ty; 
        glVertices[index + 11] = vertex.tanx;  glVertices[index + 12] = vertex.tany; glVertices[index + 13] = vertex.tanz;
        glVertices[index + 14] = vertex.bitanx; glVertices[index + 15] = vertex.bitany; glVertices[index + 16] = vertex.bitanz;
        index += 17;
    }
    // Vertex buffer
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 17, glVertices, GL_DYNAMIC_DRAW);
    delete[] glVertices;
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)0);
    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(3 * sizeof(float)));
    // normal attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(6 * sizeof(float)));
    // texture coordinates attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(9 * sizeof(float)));
    // tangent attribute
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(11 * sizeof(float)));
    // bitangent attribute
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(14 * sizeof(float)));
    // Unbind VBO
    unbind();
}

/*
TODO: Refractor repeated code
*/
void VertexBuffer::initBuffer(std::vector<Vec3f>& vertices, std::vector<unsigned int> indices) {
    // Load vertices
    unsigned int index = 0;
    float* glVertices = new float[vertices.size() * 17];
    for(Vec3f& vertex : vertices) {
        glVertices[index] = vertex.x;   glVertices[index + 1] = vertex.y;   glVertices[index + 2] = vertex.z;
        glVertices[index + 3] = vertex.r;   glVertices[index + 4] = vertex.g;   glVertices[index + 5] = vertex.b;
        glVertices[index + 6] = vertex.nx;   glVertices[index + 7] = vertex.ny;   glVertices[index + 8] = vertex.nz;
        glVertices[index + 9] = vertex.tx;  glVertices[index + 10] = vertex.ty; 
        glVertices[index + 11] = vertex.tanx;  glVertices[index + 12] = vertex.tany; glVertices[index + 13] = vertex.tanz;
        glVertices[index + 14] = vertex.bitanx; glVertices[index + 15] = vertex.bitany; glVertices[index + 16] = vertex.bitanz;
        index += 17;
    }
    // Vertex buffer
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 17, glVertices, GL_DYNAMIC_DRAW);
    delete[] glVertices;
    // Index Buffer
    if(hasIndexBuffer) indexBuffer = std::make_shared<IndexBuffer>(indices);
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)0);
    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(3 * sizeof(float)));
    // normal attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(6 * sizeof(float)));
    // texture coordinates attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(9 * sizeof(float)));
    // tangent attribute
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(11 * sizeof(float)));
    // bitangent attribute
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(14 * sizeof(float)));
    // Unbind VBO
    unbind();
}

/*
TODO: Refractor repeated code
*/
// Needed -> Buffer interface
void VertexBuffer::initBuffer() { 
    // Vertex buffer
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, length * sizeof(float) * 17, nullptr, GL_DYNAMIC_DRAW);
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)0);
    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(3 * sizeof(float)));
    // normal attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(6 * sizeof(float)));
    // texture coordinates attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(9 * sizeof(float)));
    // tangent attribute
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(11 * sizeof(float)));
    // bitangent attribute
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(14 * sizeof(float)));
    // Unbind VBO
    unbind();
}

void VertexBuffer::updateVertices(std::vector<Vec3f>& vertices) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

    unsigned int index = 0;
    for(Vec3f& vertex : vertices) {
        ptr[index] = vertex.x;     ptr[index + 1] = vertex.y; ptr[index + 2] = vertex.z;
        ptr[index + 3] = vertex.r; ptr[index + 4] = vertex.g; ptr[index + 5] = vertex.b;
        ptr[index + 6] = vertex.nx; ptr[index + 7] = vertex.ny; ptr[index + 8] = vertex.nz;
        ptr[index + 9] = vertex.tx; ptr[index + 10] = vertex.ty;
        ptr[index + 11] = vertex.tanx; ptr[index + 12] = vertex.tany; ptr[index + 13] = vertex.tanz;
        ptr[index + 14] = vertex.bitanx; ptr[index + 15] = vertex.bitany; ptr[index + 16] = vertex.bitanz;
        index += 17;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::updateVertex(int pos, Vec3f newVertex) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

    int index = pos * 17;
    ptr[index] = newVertex.x;     ptr[index + 1] = newVertex.y; ptr[index + 2] = newVertex.z;
    ptr[index + 3] = newVertex.r; ptr[index + 4] = newVertex.g; ptr[index + 5] = newVertex.b;
    ptr[index + 6] = newVertex.nx; ptr[index + 7] = newVertex.ny; ptr[index + 8] = newVertex.nz;
    ptr[index + 9] = newVertex.tx; ptr[index + 10] = newVertex.ty;
    ptr[index + 11] = newVertex.tanx; ptr[index + 12] = newVertex.tany; ptr[index + 13] = newVertex.tanz;
    ptr[index + 14] = newVertex.bitanx; ptr[index + 15] = newVertex.bitany; ptr[index + 16] = newVertex.bitanz;

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::vector<Vec3f> VertexBuffer::getVertices() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

    std::vector<Vec3f> vertices;
    for(int i = 0; i < length; i ++) {
        Vec3f vertex(
            ptr[i * 17], ptr[i * 17 + 1], ptr[i * 17 + 2],
            ptr[i * 17 + 3], ptr[i * 17 + 4], ptr[i * 17 + 5],
            ptr[i * 17 + 6], ptr[i * 17 + 7], ptr[i * 17 + 8],
            ptr[i * 17 + 9], ptr[i * 17 + 10]
        );

        vertex.tanx = ptr[i * 17 + 11];
        vertex.tany = ptr[i * 17 + 12];
        vertex.tanz = ptr[i * 17 + 13];

        vertex.bitanx = ptr[i * 17 + 14];
        vertex.bitany = ptr[i * 17 + 15];
        vertex.bitanz = ptr[i * 17 + 16];

        vertices.push_back(vertex);
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vertices;
}

void VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}