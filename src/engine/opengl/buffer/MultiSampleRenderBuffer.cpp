#include "MultiSampleRenderBuffer.h"

MultiSampleRenderBuffer::MultiSampleRenderBuffer(unsigned int _width, unsigned int _height, unsigned int _samples) 
    : Buffer(), width(_width), height(_height), samples(_samples) {
    initBuffer();
}

MultiSampleRenderBuffer::~MultiSampleRenderBuffer() {
    unbind();
    glDeleteBuffers(1, &id);
}

void MultiSampleRenderBuffer::initBuffer() {
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void MultiSampleRenderBuffer::bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void MultiSampleRenderBuffer::unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}