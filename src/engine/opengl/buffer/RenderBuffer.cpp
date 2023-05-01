#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(unsigned int _width, unsigned int _height, int _internalFormat) 
    : Buffer(), width(_width), height(_height), internalFormat(_internalFormat) {
    initBuffer();
}

RenderBuffer::~RenderBuffer() {
    unbind();
    glDeleteRenderbuffers(1, &id);
}

void RenderBuffer::initBuffer() {
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
}

void RenderBuffer::bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void RenderBuffer::unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}