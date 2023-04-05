#pragma once

#include <iostream>

#include "../../../glew/glew.h"

#include "../opengl/buffer/FrameBuffer.h"
#include "../opengl/buffer/MultiSampleRenderBuffer.h"
#include "../texture/MultiSampleTexture.h"

#include "../../../glm/vec3.hpp"

class TextureRenderer {
    GENERATE_PTR(TextureRenderer)
private:
    unsigned int width, height;

    FrameBuffer::Ptr frameBuffer;
    FrameBuffer::Ptr intermediateFrameBuffer;

    MultiSampleTexture::Ptr textureColorBufferMultiSampled;
    Texture::Ptr screenTexture;

    MultiSampleRenderBuffer::Ptr rbo;

    glm::vec3 backgroundColor;
public:
    TextureRenderer(unsigned int _width, unsigned int _height) 
        : width(_width), height(_height), backgroundColor(0.1f, 0.1f, 0.1f) {
        updateViewPort(width, height);
    }

    TextureRenderer()
        : width(0), height(0), backgroundColor(0.1f, 0.1f, 0.1f) {
    }

    ~TextureRenderer() = default;
public:
    void updateViewPort(unsigned int width, unsigned int height) {

        this->width = width;
        this->height = height;

        // Configure MSAA framebuffer
        frameBuffer = FrameBuffer::New();

        // Create a multisampled color attachment texture
        textureColorBufferMultiSampled = MultiSampleTexture::New(width, height);
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

    void renderToTexture() {
        // Blending
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        // Clear frame buffer
        frameBuffer->bind();
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }

    void renderToDefault() {
         // 2. now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
        intermediateFrameBuffer->blitFrom(frameBuffer, width, height);
        // 3. now render quad with scene's visuals as its texture image
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

    inline unsigned int getTexture() { return screenTexture->getID(); }

    inline glm::vec3& getBackgroundColor() { return backgroundColor; }
};