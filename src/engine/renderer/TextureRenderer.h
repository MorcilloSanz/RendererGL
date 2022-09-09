#pragma once

#include <iostream>

#include "../../../glew/glew.h"

class TextureRenderer {
private:
    unsigned int width, height;
    unsigned int framebuffer, intermediateFrameBuffer;
    unsigned int textureColorBufferMultiSampled, screenTexture;
    unsigned int rbo;
public:
    TextureRenderer(unsigned int _width, unsigned int _height) 
        : width(_width), height(_height) {
        updateViewPort(width, height);
    }

    TextureRenderer() = default;

    ~TextureRenderer() {
        deleteBuffers();
    }
private:
    void deleteBuffers() {
        if(framebuffer != 0)                    glDeleteBuffers(1, &framebuffer);
        if(intermediateFrameBuffer != 0)        glDeleteBuffers(1, &intermediateFrameBuffer);
        if(textureColorBufferMultiSampled != 0) glDeleteBuffers(1, &textureColorBufferMultiSampled);
        if(screenTexture != 0)                  glDeleteBuffers(1, &screenTexture);
        if(rbo != 0)                            glDeleteBuffers(1, &rbo);
        framebuffer = intermediateFrameBuffer = textureColorBufferMultiSampled = screenTexture = rbo = 0;
    }
public:
    void updateViewPort(unsigned int width, unsigned int height) {
        this->width = width;
        this->height = height;
        // configure MSAA framebuffer
        // --------------------------
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // create a multisampled color attachment texture
        glGenTextures(1, &textureColorBufferMultiSampled);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
        // create a (also multisampled) renderbuffer object for depth and stencil attachments
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // configure second post-processing framebuffer
        glGenFramebuffers(1, &intermediateFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, intermediateFrameBuffer);
        // create a color attachment texture
        glGenTextures(1, &screenTexture);
        glBindTexture(GL_TEXTURE_2D, screenTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void renderToTexture() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }

    void renderToDefault() {
         // 2. now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFrameBuffer);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        // 3. now render quad with scene's visuals as its texture image
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
    }

    inline unsigned int getTexture() { return screenTexture; }
};