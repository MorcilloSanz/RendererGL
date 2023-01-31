#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "../opengl/buffer/VertexArray.h"
#include "../opengl/buffer/VertexBuffer.h"
#include "../opengl/shader/Shader.h"

class SkyBox : public Buffer {
    GENERATE_PTR(SkyBox)
private:
    VertexArray::Ptr vertexArray;
    VertexBuffer::Ptr vertexBuffer;
    std::vector<std::string> faces;
public:
    SkyBox() = default;
    // loads a cubemap texture from 6 individual texture faces
    // order:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front) 
    // -Z (back)
    // -------------------------------------------------------
    SkyBox(const std::vector<std::string>& _faces);
    ~SkyBox();
private:
    void loadCubeMap();
    void initBuffer() override;
public:
    void bind() override;
    void unbind() override;
    void draw();
public:
    inline std::vector<std::string>& getFaces() { return faces; }
    inline VertexArray::Ptr& getVertexArray() { return vertexArray; }
};