#pragma once

#include <iostream>
#include <vector>

#include "Texture.h"

class CubeMapTexture : public Texture {
    GENERATE_PTR(CubeMapTexture)
private:
    std::vector<std::string> faces;
public:
    CubeMapTexture(const std::vector<std::string>& _faces);
    CubeMapTexture() = default;
    ~CubeMapTexture() = default;
private:
    void generateTexture() override;
public:
    void bind() override;
    void unbind() override;
public:
    inline std::vector<std::string>& getFaces() { return faces; }
};