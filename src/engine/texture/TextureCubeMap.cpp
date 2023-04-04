#include "TextureCubeMap.h"

TextureCubeMap::TextureCubeMap(const std::vector<std::string>& faces) 
    : Texture() {
    type = Type::TextureCubeMap;
    generateTexture(faces);
}

void TextureCubeMap::generateTexture(const std::vector<std::string>& faces) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

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
    
    slot = 0x84C0 + count;
    count ++;
}

void TextureCubeMap::bind() {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void TextureCubeMap::unbind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}