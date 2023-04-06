#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(unsigned int _width, unsigned int _height) 
    : Buffer(), width(_width), height(_height) {
    initBuffer();
}

RenderBuffer::~RenderBuffer() {
    unbind();
    glDeleteBuffers(1, &id);
}

void RenderBuffer::initBuffer() {
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
}

void RenderBuffer::bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void RenderBuffer::unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}