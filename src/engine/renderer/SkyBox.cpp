#include "SkyBox.h"

#include "../texture/Texture.h"

#include "../Vec3.h"

std::vector<Vec3f> vertices = {        
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

SkyBox::SkyBox(const std::vector<std::string>& _faces) 
    : Buffer(), faces(_faces) {
    initBuffer();
}

SkyBox::~SkyBox() {
    unbind();
}

void SkyBox::initBuffer() {
    vertexArray = VertexArray::New();
    vertexBuffer = VertexBuffer::New(vertices);
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