#include "MultiSampleTexture.h"

MultiSampleTexture::MultiSampleTexture(unsigned int width, unsigned int height, unsigned int _samples) 
    : Texture(), samples(_samples) {
    this->width = width;
    this->height = height;
    generateTexture();
}

void MultiSampleTexture::generateTexture() {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, width, height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    slot = 0x84C0 + count;
    count ++;
}

void MultiSampleTexture::bind() {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
}

void MultiSampleTexture::unbind() {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}