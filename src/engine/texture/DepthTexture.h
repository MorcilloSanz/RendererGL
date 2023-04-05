#pragma once

#include "Texture.h"

class DepthTexture : public Texture {
    GENERATE_PTR(DepthTexture)
public:
    DepthTexture(int _width, int _height);
    DepthTexture() = default;
    ~DepthTexture() = default;
private:
    void generateTexture() override;
};