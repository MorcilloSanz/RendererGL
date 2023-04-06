#include "MultiSampleRenderBuffer.h"

MultiSampleRenderBuffer::MultiSampleRenderBuffer(unsigned int width, unsigned int height, unsigned int _samples) 
    : RenderBuffer(width, height), samples(_samples) {
    initBuffer();
}

void MultiSampleRenderBuffer::initBuffer() {
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}