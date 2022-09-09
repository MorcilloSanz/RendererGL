#include "VertexArray.h"

VertexArray::VertexArray() 
    : Buffer() {
    initBuffer();
}

VertexArray::VertexArray(const VertexArray& vertexArray) {
    id = vertexArray.id;
}

VertexArray::VertexArray(VertexArray&& vertexArray) noexcept {
    id = vertexArray.id;
}

VertexArray& VertexArray::operator=(const VertexArray& vertexArray) {
    id = vertexArray.id;
    return *this;
}

VertexArray::~VertexArray() {
    unbind();
    glDeleteVertexArrays(1, &id);
}

void VertexArray::initBuffer() {
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
}

void VertexArray::bind() {
    glBindVertexArray(id);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}