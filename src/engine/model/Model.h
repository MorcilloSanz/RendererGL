#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../group/Group.h"
#include "../texture/Texture.h"
#include "../lighting/Material.h"

class Model : public Group {
private:
    std::string directory, path;
    std::vector<std::shared_ptr<Texture>> texturesLoaded;
    bool gammaCorrection;
public:
    Model(const std::string& _path, bool _gammaCorrection = false);
    Model() = default;
private:
    void loadModel();
    void processNode(aiNode *node, const aiScene *scene);
    std::shared_ptr<Polytope> processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName);
};