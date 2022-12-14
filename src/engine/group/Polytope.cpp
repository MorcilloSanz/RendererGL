#include "Polytope.h"

#include "../../../glew/glew.h"

Polytope::Polytope(size_t length) 
    : vertexLength(length), modelMatrix(1.f), indicesLength(0), selected(false) {
    initPolytope(length);
}

Polytope::Polytope(std::vector<Vec3f>& vertices)
    : vertexLength(vertices.size()), modelMatrix(1.f), indicesLength(0), selected(false) {
    initPolytope(vertices);
}

Polytope::Polytope(std::vector<Vec3f>& vertices, std::vector<unsigned int>& indices) 
    : vertexLength(vertices.size()), modelMatrix(1.f), indicesLength(indices.size()), selected(false) {
    initPolytope(vertices, indices);
}

/*
*  TODO: Refractor, repeated code
*/
void Polytope::initPolytope(size_t length) {
    vertexArray = std::make_shared<VertexArray>();
    vertexBuffer = std::make_shared<VertexBuffer>(length);
    material = Material(MATERIAL_DIFFUSE, MATERIAL_SPECULAR, MATERIAL_SHININESS);
    unbind();
}

void Polytope::initPolytope(std::vector<Vec3f>& vertices) {
    vertexArray = std::make_shared<VertexArray>();
    vertexBuffer = std::make_shared<VertexBuffer>(vertices);
    material = Material(MATERIAL_DIFFUSE, MATERIAL_SPECULAR, MATERIAL_SHININESS);
    unbind();
}

void Polytope::initPolytope(std::vector<Vec3f>& vertices, std::vector<unsigned int>& indices) {
    vertexArray = std::make_shared<VertexArray>();
    vertexBuffer = std::make_shared<VertexBuffer>(vertices, indices);
    material = Material(MATERIAL_DIFFUSE, MATERIAL_SPECULAR, MATERIAL_SHININESS);
    unbind();
}

void Polytope::bind() {
    if(vertexArray != nullptr && vertexBuffer != nullptr)
        vertexArray->bind();
}

void Polytope::unbind() {
    if(vertexArray != nullptr && vertexBuffer != nullptr)
        vertexArray->unbind();
}

void Polytope::updateVertices(std::vector<Vec3f>& vertices) {
    if(vertexBuffer != nullptr) {
        vertexBuffer->updateVertices(vertices);
        vertexLength = vertices.size();
    }
}

void Polytope::updateVertex(int pos, Vec3f newVertex) {
    if(vertexBuffer != nullptr)
        vertexBuffer->updateVertex(pos, newVertex);
}

void Polytope::updateIndices(std::vector<unsigned int>& indices) {
    if(vertexBuffer != nullptr && vertexBuffer->getIndexBuffer() != nullptr) {
        vertexBuffer->getIndexBuffer()->updateIndices(indices);
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