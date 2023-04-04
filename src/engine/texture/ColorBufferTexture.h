#pragma once

#include "Texture.h"

class ColorBufferTexture : public Texture {
    GENERATE_PTR(ColorBufferTexture)
public:
    ColorBufferTexture(int _width, int _height);
    ColorBufferTexture() = default;
    ~ColorBufferTexture() = default;
private:
    void generateTexture();
};