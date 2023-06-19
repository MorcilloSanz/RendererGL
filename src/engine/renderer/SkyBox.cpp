#include "SkyBox.h"

#include "engine/texture/Texture.h"

#include "engine/Vec3.h"

SkyBox::SkyBox(const std::vector<std::string>& _faces) 
    : Buffer(), faces(_faces) {
    initBuffer();
}

SkyBox::~SkyBox() {
    unbind();
}

void SkyBox::initBuffer() {

    std::vector<Vec3f> sbv = {        
        Vec3f(-1.0f,  1.0f, -1.0f), Vec3f(-1.0f, -1.0f, -1.0f), Vec3f(1.0f, -1.0f, -1.0f ),
        Vec3f(1.0f, -1.0f, -1.0f ), Vec3f(1.0f,  1.0f, -1.0f ), Vec3f(-1.0f,  1.0f, -1.0f),
        Vec3f(-1.0f, -1.0f,  1.0f), Vec3f(-1.0f, -1.0f, -1.0f), Vec3f(-1.0f,  1.0f, -1.0f),
        Vec3f(-1.0f,  1.0f, -1.0f), Vec3f(-1.0f,  1.0f,  1.0f), Vec3f(-1.0f, -1.0f,  1.0f),
        Vec3f(1.0f, -1.0f, -1.0f), Vec3f(1.0f, -1.0f,  1.0f), Vec3f(1.0f,  1.0f,  1.0f),
        Vec3f(1.0f,  1.0f,  1.0f), Vec3f(1.0f,  1.0f, -1.0f), Vec3f(1.0f, -1.0f, -1.0f),
        Vec3f(-1.0f, -1.0f,  1.0f), Vec3f(-1.0f,  1.0f,  1.0f), Vec3f(1.0f,  1.0f,  1.0f ),
        Vec3f(1.0f,  1.0f,  1.0f ), Vec3f(1.0f, -1.0f,  1.0f ), Vec3f(-1.0f, -1.0f,  1.0f),
        Vec3f(-1.0f,  1.0f, -1.0f), Vec3f(1.0f,  1.0f, -1.0f ), Vec3f(1.0f,  1.0f,  1.0f ),
        Vec3f(1.0f,  1.0f,  1.0f ), Vec3f(-1.0f,  1.0f,  1.0f), Vec3f(-1.0f,  1.0f, -1.0f),
        Vec3f(-1.0f, -1.0f, -1.0f), Vec3f(-1.0f, -1.0f,  1.0f), Vec3f(1.0f, -1.0f, -1.0f ),
        Vec3f(1.0f, -1.0f, -1.0f ), Vec3f(-1.0f, -1.0f,  1.0f), Vec3f(1.0f, -1.0f,  1.0f)
    };

    vertexArray = VertexArray::New();
    vertexBuffer = VertexBuffer::New(sbv);
    cubeMap = CubeMapTexture::New(faces);
    unbind();
}

void SkyBox::bind() {
    vertexArray->bind();
}

void SkyBox::unbind() {
    vertexArray->unbind();
}

void SkyBox::draw() {
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}