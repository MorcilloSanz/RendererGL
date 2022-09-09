#include "IndexBuffer.h"

#include <string.h>

IndexBuffer::IndexBuffer() : Buffer() { }

IndexBuffer::IndexBuffer(const std::vector<unsigned int> _indices)
    : Buffer(), indices(_indices) {
    initBuffer();
}

IndexBuffer::IndexBuffer(const IndexBuffer& indexBuffer) :
    indices(indexBuffer.indices) {
    id = indexBuffer.id;
}

IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept 
    : indices(std::move(indexBuffer.indices)) {
    id = indexBuffer.id;
}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& indexBuffer) {
    indices = indexBuffer.indices;
    id = indexBuffer.id;
    return *this;
}

IndexBuffer::~IndexBuffer() {
    unbind();
    glDeleteBuffers(1, &id);
}

void IndexBuffer::initBuffer() {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), &indices[0], GL_DYNAMIC_DRAW);
}

void IndexBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::updateIndices(const std::vector<unsigned int>& indices, bool copy2memory) {
    if(copy2memory) this->indices = indices;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    unsigned int* ptr = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, &indices[0], indices.size());
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}