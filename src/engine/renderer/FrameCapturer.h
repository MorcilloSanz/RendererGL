#pragma once

#include <iostream>

#include <GL/glew.h>

#include <glm/vec3.hpp>

#include "engine/opengl/buffer/FrameBuffer.h"
#include "engine/opengl/buffer/MultiSampleRenderBuffer.h"
#include "engine/texture/MultiSampleTexture.h"

class FrameCapturer {
    GENERATE_PTR(FrameCapturer)
private:
    unsigned int width, height;
    unsigned int samples;

    FrameBuffer::Ptr frameBuffer;
    FrameBuffer::Ptr intermediateFrameBuffer;

    MultiSampleTexture::Ptr textureColorBufferMultiSampled;
    Texture::Ptr screenTexture;

    MultiSampleRenderBuffer::Ptr rbo;

    glm::vec3 backgroundColor;
public:
    FrameCapturer(unsigned int _width, unsigned int _height, unsigned int _samples = 4) 
        : width(_width), height(_height), samples(_samples), backgroundColor(0.1f, 0.1f, 0.1f) {
        updateViewPort(width, height, samples);
    }

    FrameCapturer()
        : width(0), height(0), backgroundColor(0.1f, 0.1f, 0.1f) {
    }

    ~FrameCapturer() = default;
public:
    void updateViewPort(unsigned int width, unsigned int height, unsigned int samples = 4) {

        this->width = width;
        this->height = height;
        this->samples = samples;

        // Configure MSAA framebuffer
        frameBuffer = FrameBuffer::New();

        // Create a multisampled color attachment texture
        textureColorBufferMultiSampled = MultiSampleTexture::New(width, height, samples);
        frameBuffer->toTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled->getID());

        // Create a (also multisampled) renderbuffer object for depth and stencil attachments
        rbo = MultiSampleRenderBuffer::New(width, height);
        frameBuffer->setRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT, rbo->getID());

        if (!frameBuffer->isComplete()) std::cout << "Error: framebuffer is not complete!" << std::endl;
        
        frameBuffer->unbind();

        // configure second post-processing framebuffer
        intermediateFrameBuffer = FrameBuffer::New();

        // create a color attachment texture
        screenTexture = Texture::New(width, height);

        intermediateFrameBuffer->toTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture->getID());

        if (!intermediateFrameBuffer->isComplete()) std::cout << "Error: Intermediate framebuffer is not complete!" << std::endl;

        intermediateFrameBuffer->unbind();
    }

    void startCapturing() {

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        frameBuffer->bind();
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }

    void finishCapturing() {

        intermediateFrameBuffer->blitFrom(frameBuffer, width, height);
        intermediateFrameBuffer->unbind();

        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
    }

    void setBackgroundColor(float r, float g, float b) {
        backgroundColor.r = r;
        backgroundColor.g = g;
        backgroundColor.b = b;
    }

    inline Texture::Ptr& getTexture() { return screenTexture; }

    inline glm::vec3& getBackgroundColor() { return backgroundColor; }
};