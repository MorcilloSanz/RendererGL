#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "../opengl/buffer/VertexArray.h"
#include "../opengl/buffer/VertexBuffer.h"
#include "../opengl/shader/Shader.h"

class SkyBox : public Buffer {
private:
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::vector<std::string> faces;
    std::shared_ptr<ShaderProgram> shaderProgram;
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
    inline std::shared_ptr<ShaderProgram>& getShaderProgram() { return shaderProgram; }
    inline std::vector<std::string>& getFaces() { return faces; }

    inline std::shared_ptr<VertexArray>& getVertexArray() { return vertexArray; }
};