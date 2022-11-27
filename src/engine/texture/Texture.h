#pragma once

#include <iostream>

#include "../../../glew/glew.h"

class Texture {
public:
    enum class Type {
        None, TextureAmbient, TextureDiffuse, TextureSpecular, TextureNormal, TextureHeight
    };
    static int textureUnits;
protected:
    static unsigned int count;
    int slot;
    unsigned int id;
    int width, height, bpp;
    bool flip;
    std::string path;
    Type type;
public:
    Texture(const std::string& _path, const Type& _type = Type::None, bool _flip = true);
    Texture();
    Texture(const Texture& texture);
    Texture(Texture&& texture) noexcept;
    ~Texture();
    Texture& operator=(const Texture& texture);
private:
    inline void initTextureUnits() { glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Texture::textureUnits); }
    void generateTexture();
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