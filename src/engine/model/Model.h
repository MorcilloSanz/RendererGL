#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "engine/group/Group.h"
#include "engine/texture/Texture.h"
#include "engine/lighting/Material.h"

#include "engine/ptr.h"

class Model : public Group {
    GENERATE_PTR(Model)
private:
    std::string directory, path;
    std::vector<Texture::Ptr> texturesLoaded;
    bool gammaCorrection;
public:
    Model(const std::string& _path, bool _gammaCorrection = false);
    Model() = default;
private:
    void loadModel();
    void processNode(aiNode *node, const aiScene *scene);
    Polytope::Ptr processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture::Ptr> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName);
};