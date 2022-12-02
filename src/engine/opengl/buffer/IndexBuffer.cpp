#include "IndexBuffer.h"

#include <string.h>

IndexBuffer::IndexBuffer() : Buffer() { }

IndexBuffer::IndexBuffer(const std::vector<unsigned int> indices)
    : Buffer(), length(indices.size()) {
    initBuffer(indices);
}

IndexBuffer::IndexBuffer(const IndexBuffer& indexBuffer) 
    : length(indexBuffer.length) {
    id = indexBuffer.id;
}

IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept 
    : length(indexBuffer.length) {
    id = indexBuffer.id;
}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& indexBuffer) {
    id = indexBuffer.id;
    length = indexBuffer.length;
    return *this;
}

IndexBuffer::~IndexBuffer() {
    unbind();
    glDeleteBuffers(1, &id);
}

void IndexBuffer::initBuffer(std::vector<unsigned int> indices) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices), &indices[0], GL_DYNAMIC_DRAW);
}

void IndexBuffer::initBuffer() { }

void IndexBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::updateIndices(const std::vector<unsigned int>& indices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    unsigned int* ptr = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, &indices[0], indices.size());
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::vector<unsigned int> IndexBuffer::getIndices() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    unsigned int* ptr = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);

    std::vector<unsigned int> indices;
    for(int i = 0; i < length; i ++) indices.push_back(ptr[i]);
    
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return indices;
}