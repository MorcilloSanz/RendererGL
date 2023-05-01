#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
    : Buffer() {
    initBuffer();
}

FrameBuffer::~FrameBuffer() {
    unbind();
    glDeleteFramebuffers(1, &id);
}

void FrameBuffer::initBuffer() {
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::toTexture(int attachment, int texturePrimitive, int textureID) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texturePrimitive, textureID, 0);
}

void FrameBuffer::blitFrom(FrameBuffer::Ptr& frameBuffer, unsigned int width, unsigned int height) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer->getID());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void FrameBuffer::setRenderBuffer(int attachment, int renderBufferID) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBufferID);
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBuffer::isComplete() {
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}