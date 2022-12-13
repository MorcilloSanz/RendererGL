#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

unsigned int Texture::count = 0;
int Texture::textureUnits = 0;

Image readImage(const std::string& path) {
	Image image;
	image.data = stbi_load(path.c_str(), &image.width, &image.height, &image.bpp, STBI_rgb);
	return image;
}

Texture::Texture(const std::string& _path, const Type& _type, bool _flip) 
	: path(_path), id(0), width(0), height(0), bpp(0), slot(0), type(_type), flip(_flip) {
	initTextureUnits();
	//if(count < textureUnits) generateTexture();
	generateTexture();
}

Texture::Texture() 
	: id(0), width(0), height(0), bpp(0), path(""), slot(0), type(Type::None) {
	initTextureUnits();
}

Texture::Texture(const Texture& texture)
	: path(texture.path), id(texture.id), width(texture.width), 
	height(texture.height), bpp(texture.bpp), slot(texture.slot) {
}

Texture::Texture(Texture&& texture) noexcept
	: path(std::move(texture.path)), id(texture.id), width(texture.width), 
    height(texture.height), bpp(texture.bpp), slot(texture.slot) {
}

Texture::~Texture() {
	unbind();
	glActiveTexture(0);
	glDeleteTextures(1, &id);
}

Texture& Texture::operator=(const Texture& texture) {
	id = texture.id;
	width = texture.width;
	height = texture.height;
	bpp = texture.bpp;
    slot = texture.slot;
    path = texture.path;
	return *this;
}

void Texture::generateTexture() {
    stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &bpp, STBI_rgb_alpha);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (data) stbi_image_free(data);

	slot = 0x84C0 + count;
	count++;
}

void Texture::bind() {
    glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}