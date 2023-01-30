#pragma once

#include <iostream>

#include "../../../glew/glew.h"

struct Image {
    unsigned char* data;
    int width, height, bpp;

    ~Image() {
        if(data != nullptr) free(data);
    }
};

Image readImage(const std::string& path);

class Texture {
public:
    enum class Type : int {
        None = -1, TextureAmbient = 0, TextureDiffuse = 1, TextureSpecular = 2, 
        TextureNormal = 3, TextureHeight = 4, TextureEmission = 5
    };
    static int textureUnits;
    static unsigned int count;
protected:
    int slot;
    unsigned int id;
    int width, height, bpp;
    bool flip;
    std::string path;
    Type type;
public:
    Texture(const std::string& _path, const Type& _type = Type::TextureDiffuse, bool _flip = true);
    Texture(unsigned char* buffer, const Type& _type = Type::TextureDiffuse);
    Texture(const Texture& texture);
    Texture(Texture&& texture) noexcept;
    Texture();
    ~Texture();
    Texture& operator=(const Texture& texture);
protected:
    inline void initTextureUnits() { glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Texture::textureUnits); }
    void generateTextureFromFile(const std::string& path);
    void generateTextureFromBuffer(unsigned char* buffer);
public:
    virtual void bind();
    virtual void unbind();
public:
    inline void setID(unsigned int id) { this->id = id; }
    inline unsigned int getID() const { return id; }

    inline void setPath(const std::string& path) { this->path = path; }
    inline const std::string& getPath() { return path; }

    inline void setSlot(int slot) { this->slot = slot; }
	inline int getSlot() const { return slot; }

    inline void setType(const Type& type) { this->type = type; }
    inline Type& getType() { return type; }

    static unsigned int getCount() { return count; }
};