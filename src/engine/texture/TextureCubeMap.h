#pragma once

#include <iostream>
#include <vector>

#include "Texture.h"

class TextureCubeMap : public Texture {
    GENERATE_PTR(TextureCubeMap)
public:
    TextureCubeMap(const std::vector<std::string>& faces);
    TextureCubeMap() = default;
    ~TextureCubeMap() = default;
private:
    void generateTexture(const std::vector<std::string>& faces);
public:
    void bind() override;
    void unbind() override;
};