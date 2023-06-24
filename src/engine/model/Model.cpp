#include "Model.h"

Model::Model(const std::string& _path, bool _pbr) 
    : path(_path), pbr(_pbr) {
    loadModel();
}

void Model::loadModel() {
    
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));
    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {

    // process each mesh located at the current nodeDynamicPolytope::New
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Polytope::Ptr polytope = processMesh(mesh, scene); // may be freed?
        add(polytope);
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++) processNode(node->mChildren[i], scene);
}

Polytope::Ptr Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    // data to fill
    std::vector<Vec3f> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture::Ptr> textures;   

    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vec3f vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vertex.x = vector.x = mesh->mVertices[i].x;
        vertex.y = vector.y = mesh->mVertices[i].y;
        vertex.z = vector.z = mesh->mVertices[i].z;

        vertex.r = vertex.g = vertex.b = 1.0f;

        // normals
        if (mesh->HasNormals()) {
            vertex.nx = vector.x = mesh->mNormals[i].x;
            vertex.ny = vector.y = mesh->mNormals[i].y;
            vertex.nz = vector.z = mesh->mNormals[i].z;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0])  {

            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertex.tx = vec.x = mesh->mTextureCoords[0][i].x; 
            vertex.ty = vec.y = mesh->mTextureCoords[0][i].y;

            // tangent
            vertex.tanx = vector.x = mesh->mTangents[i].x;
            vertex.tany = vector.y = mesh->mTangents[i].y;
            vertex.tanz = vector.z = mesh->mTangents[i].z;

            // bitangent
            vertex.bitanx = vector.x = mesh->mBitangents[i].x;
            vertex.bitany = vector.y = mesh->mBitangents[i].y;
            vertex.bitanz = vector.z = mesh->mBitangents[i].z;

            vertex.hasTangents = true;
        }
        else vertex.tx = vertex.ty = 0.0f;
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);        
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    

    // Blinn-Phong materials
    if(!pbr) {
        std::vector<Texture::Ptr> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture::Ptr>  specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<Texture::Ptr>  normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<Texture::Ptr>  heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }
    // PBR materials
    else {
        std::vector<Texture::Ptr> pbrAlbedoMaps = loadMaterialTextures(material, aiTextureType_BASE_COLOR, "pbr_texture_albedo");
        textures.insert(textures.end(), pbrAlbedoMaps.begin(), pbrAlbedoMaps.end());

        std::vector<Texture::Ptr> pbrMetalnessMaps = loadMaterialTextures(material, aiTextureType_METALNESS, "pbr_texture_metallic");
        textures.insert(textures.end(), pbrMetalnessMaps.begin(), pbrMetalnessMaps.end());

        std::vector<Texture::Ptr>  normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "pbr_texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<Texture::Ptr> pbrEmissionMaps = loadMaterialTextures(material, aiTextureType_EMISSION_COLOR, "pbr_texture_emission");
        textures.insert(textures.end(), pbrEmissionMaps.begin(), pbrEmissionMaps.end());

        std::vector<Texture::Ptr> pbrRoughnessMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "pbr_texture_roughness");
        textures.insert(textures.end(), pbrRoughnessMaps.begin(), pbrRoughnessMaps.end());

        std::vector<Texture::Ptr> pbrAmbientOcclusion = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, "pbr_texture_ao");
        textures.insert(textures.end(), pbrAmbientOcclusion.begin(), pbrAmbientOcclusion.end());
    }

    // return a mesh object created from the extracted mesh data
    Polytope::Ptr polytope = Polytope::New(vertices, indices, false);
    for(auto& texture : textures) polytope->addTexture(texture);
    return polytope;
}

std::vector<Texture::Ptr> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName) {

    std::vector<Texture::Ptr> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {

        aiString str;
        mat->GetTexture(type, i, &str);

        std::string texturePath = directory + "/" + str.C_Str();
        Texture::Ptr texture = Texture::New(texturePath, Texture::Type::None, false);

        // Phong lighting
        if(!pbr) {
            if(typeName == "texture_ambient") texture->setType(Texture::Type::TextureAmbient);
            else if(typeName == "texture_diffuse") texture->setType(Texture::Type::TextureDiffuse);
            else if(typeName == "texture_specular") texture->setType(Texture::Type::TextureSpecular);
            else if(typeName == "texture_height") texture->setType(Texture::Type::TextureHeight);
            else if(typeName == "texture_normal") texture->setType(Texture::Type::TextureNormal);
        }
        // PBR
        else {
            if(typeName == "pbr_texture_albedo") texture->setType(Texture::Type::TextureAlbedo);
            else if(typeName == "pbr_texture_metallic") texture->setType(Texture::Type::TextureMetallic);
            else if(typeName == "pbr_texture_normal") texture->setType(Texture::Type::TextureNormal);
            else if(typeName == "pbr_texture_roughness") texture->setType(Texture::Type::TextureRoughness);
            else if(typeName == "pbr_texture_ao") texture->setType(Texture::Type::TextureAmbientOcclusion);
            else if(typeName == "pbr_texture_emission") texture->setType(Texture::Type::TextureEmission);
        }

        bool contained = false;
        for(auto& tex : textures) {
            if(tex->getPath() == texture->getPath()) {
                contained = true;
                break;
            }
        }
        if(!contained) textures.push_back(texture);
    }
    return textures;
}