#include "SkyBox.h"

#include "../texture/Texture.h"

#include "../Vec3.h"

std::vector<Vec3f> vertices = {        
    Vec3f(-1.0f,  1.0f, -1.0f),
    Vec3f(-1.0f, -1.0f, -1.0f),
    Vec3f(1.0f, -1.0f, -1.0f ),
    Vec3f(1.0f, -1.0f, -1.0f ),
    Vec3f(1.0f,  1.0f, -1.0f ),
    Vec3f(-1.0f,  1.0f, -1.0f),

    Vec3f(-1.0f, -1.0f,  1.0f),
    Vec3f(-1.0f, -1.0f, -1.0f),
    Vec3f(-1.0f,  1.0f, -1.0f),
    Vec3f(-1.0f,  1.0f, -1.0f),
    Vec3f(-1.0f,  1.0f,  1.0f),
    Vec3f(-1.0f, -1.0f,  1.0f),

    Vec3f(1.0f, -1.0f, -1.0f),
    Vec3f(1.0f, -1.0f,  1.0f),
    Vec3f(1.0f,  1.0f,  1.0f),
    Vec3f(1.0f,  1.0f,  1.0f),
    Vec3f(1.0f,  1.0f, -1.0f),
    Vec3f(1.0f, -1.0f, -1.0f),

    Vec3f(-1.0f, -1.0f,  1.0f),
    Vec3f(-1.0f,  1.0f,  1.0f),
    Vec3f(1.0f,  1.0f,  1.0f ),
    Vec3f(1.0f,  1.0f,  1.0f ),
    Vec3f(1.0f, -1.0f,  1.0f ),
    Vec3f(-1.0f, -1.0f,  1.0f),

    Vec3f(-1.0f,  1.0f, -1.0f),
    Vec3f(1.0f,  1.0f, -1.0f ),
    Vec3f(1.0f,  1.0f,  1.0f ),
    Vec3f(1.0f,  1.0f,  1.0f ),
    Vec3f(-1.0f,  1.0f,  1.0f),
    Vec3f(-1.0f,  1.0f, -1.0f),

    Vec3f(-1.0f, -1.0f, -1.0f),
    Vec3f(-1.0f, -1.0f,  1.0f),
    Vec3f(1.0f, -1.0f, -1.0f ),
    Vec3f(1.0f, -1.0f, -1.0f ),
    Vec3f(-1.0f, -1.0f,  1.0f),
    Vec3f(1.0f, -1.0f,  1.0f)
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
    unbind();
    loadCubeMap();
}

void SkyBox::bind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    vertexArray->bind();
}

void SkyBox::unbind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    vertexArray->unbind();
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
void SkyBox::loadCubeMap() {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++) {
        Image image = readImage(faces[i]);
        if (image.data)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
        else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            return;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    unbind();
}

void SkyBox::draw() {
    glDepthFunc(GL_LEQUAL);
    vertexArray->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    vertexArray->unbind();
    glDepthFunc(GL_LESS);
}