#pragma once

#include <iostream>
#include <vector>

#include "Texture.h"

class CubeMapTexture : public Texture {
    GENERATE_PTR(CubeMapTexture)
public:
    CubeMapTexture(const std::vector<std::string>& faces);
    CubeMapTexture() = default;
    ~CubeMapTexture() = default;
private:
    void generateTexture(const std::vector<std::string>& faces);
public:
    void bind() override;
    void unbind() override;
};