#include "Polytope.h"

#include "../../../glew/glew.h"

#define MATERIAL_DIFFUSE glm::vec3(1.0f, 1.0f, 1.0f)
#define MATERIAL_SPECULAR glm::vec3(1.0f, 1.0f, 1.0f)
#define MATERIAL_SHININESS 32.f

Polytope::Polytope(const std::vector<Vec3f>& vertices)
    : vertexLength(vertices.size()), modelMatrix(1.f), indicesLength(0) {
    vertexArray = std::make_shared<VertexArray>();
    vertexBuffer = std::make_shared<VertexBuffer>(vertices);
    material = Material(MATERIAL_DIFFUSE, MATERIAL_SPECULAR, MATERIAL_SHININESS);
    unbind();
}

Polytope::Polytope(const std::vector<Vec3f>& vertices, const std::vector<unsigned int>& indices) 
    : vertexLength(vertices.size()), modelMatrix(1.f), indicesLength(indices.size()) {
    vertexArray = std::make_shared<VertexArray>();
    vertexBuffer = std::make_shared<VertexBuffer>(vertices, indices);
    material = Material(MATERIAL_DIFFUSE, MATERIAL_SPECULAR, MATERIAL_SHININESS);
    unbind();
}

void Polytope::bind() {
    if(vertexArray != nullptr && vertexBuffer != nullptr) {
        vertexArray->bind();
    }
}

void Polytope::unbind() {
    if(vertexArray != nullptr && vertexBuffer != nullptr) {
        vertexArray->unbind();
        vertexBuffer->unbind();
    }
}

void Polytope::updateVertices(std::vector<Vec3f>& vertices, bool copy2memory) {
    if(vertexBuffer != nullptr) {
        vertexBuffer->updateVertices(vertices, copy2memory);
        vertexLength = vertices.size();
    }
}

void Polytope::updateIndices(std::vector<unsigned int>& indices, bool copy2memory) {
    if(vertexBuffer != nullptr && vertexBuffer->getIndexBuffer() != nullptr) {
        vertexBuffer->getIndexBuffer()->updateIndices(indices, copy2memory);
        indicesLength = indices.size();
    }
}

void Polytope::draw(unsigned int primitive, bool showWire) {
    bind();
    if(!showWire)   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(!vertexBuffer->HasIndexBuffer()) glDrawArrays(primitive, 0, vertexLength);
    else    glDrawElements(primitive, indicesLength, GL_UNSIGNED_INT, 0);
    unbind();
}