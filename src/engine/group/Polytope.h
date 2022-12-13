#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "../../../glm/vec3.hpp"
#include "../../../glm/vec4.hpp"
#include "../../../glm/mat4x4.hpp"
#include "../../../glm/gtc/type_ptr.hpp"

#include "../opengl/buffer/VertexArray.h"
#include "../opengl/buffer/VertexBuffer.h"

#include "../lighting/Material.h"

#include "../texture/Texture.h"

class Polytope {
protected:
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::vector<std::shared_ptr<Texture>> textures;
    unsigned int vertexLength, indicesLength;
    Material material;
    glm::mat4 modelMatrix;
public:
    Polytope(size_t length);
    Polytope(std::vector<Vec3f>& vertices);
    Polytope(std::vector<Vec3f>& vertices, std::vector<unsigned int>& indices);
    Polytope() = default;
    ~Polytope() = default;
public:
    void initPolytope(size_t length);
    void initPolytope(std::vector<Vec3f>& vertices);
    void initPolytope(std::vector<Vec3f>& vertices, std::vector<unsigned int>& indices);
    void bind();
    void unbind();
    void updateVertices(std::vector<Vec3f>& vertices);
    void updateVertex(int pos, Vec3f newVertex);
    void updateIndices(std::vector<unsigned int>& indices);
    void bindTexture();
    void unbindTexture();
    void draw(unsigned int primitive, bool showWire = false);
public:
    inline void translate(const glm::vec3& v) { modelMatrix = glm::translate(modelMatrix, v); }
    inline void rotate(float degrees, const glm::vec3& axis) { modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), axis); }
    inline void scale(const glm::vec3& s) { modelMatrix = glm::scale(modelMatrix, s); }

    inline std::shared_ptr<VertexArray>& getVertexArray() { return vertexArray; }
    inline std::shared_ptr<VertexBuffer>& getVertexBuffer() { return vertexBuffer; }
    inline std::shared_ptr<IndexBuffer>& getIndexBuffer() { return vertexBuffer->getIndexBuffer(); }

    inline void addTexture(const std::shared_ptr<Texture>& texture) { textures.push_back(texture); }
    inline std::vector<std::shared_ptr<Texture>>& getTextures() { return textures; }

    inline unsigned int getVertexLength() const { return vertexLength; }

    inline void setModelMatrix(const glm::mat4& modelMatrix) { this->modelMatrix = modelMatrix; }
    inline glm::mat4& getModelMatrix() { return modelMatrix; }

    inline void setVetexArray(const std::shared_ptr<VertexArray>& vertexArray) { this->vertexArray = vertexArray; }
    inline void setVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) { this->vertexBuffer = vertexBuffer; }
    inline void setVertexLength(unsigned int vertexLength) { this->vertexLength = vertexLength; }

    inline void setMaterial(const Material& material) { this->material = material; }
    inline Material& getMaterial() { return material; }

    inline std::vector<Vec3f> getVertices() { return vertexBuffer->getVertices(); }
    inline std::vector<unsigned int> getIndices() { return vertexBuffer->getIndexBuffer()->getIndices(); }
};