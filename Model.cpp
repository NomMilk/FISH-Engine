#include "Model.h"
#include <iostream>
#include <vector>
#include <filesystem>
#include <unistd.h>
#include <cctype>

using namespace std;

Model::Model(const char* path) {
    loadModel(path);
}

Model::~Model() {
    for (auto& mesh : meshes) {
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
    }
    
    for (size_t i = 0; i < loadedTextures.size(); i++) {
        if (loadedTextures[i]) {
            delete loadedTextures[i];
            loadedTextures[i] = nullptr;
        }
    }
    
    textures.clear();
    loadedTextures.clear();
    meshes.clear();
}

void Model::Draw(Shader& shader) {
    shader.Activate();
    glUniform1i(glGetUniformLocation(shader.ID, "useTexture"), true);
    
    for (size_t i = 0; i < meshes.size(); i++) {
        Mesh& mesh = meshes[i];
        if (mesh.materialIndex < textures.size() && textures[mesh.materialIndex] != nullptr) {
            textures[mesh.materialIndex]->Bind();
        } else if (!loadedTextures.empty()) {
            loadedTextures[0]->Bind();
        }
        
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void Model::loadModel(const string& path) {
    directory = "models/";    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | 
        aiProcess_FlipUVs | 
        aiProcess_GenNormals);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
        
    textures.resize(scene->mNumMaterials, nullptr);
    // here to check if 3d model is loaded
    if (scene->HasMaterials()) {
        for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
            aiMaterial* material = scene->mMaterials[i];
            loadMaterialTextures(material, aiTextureType_DIFFUSE, i);
        }
    }
    
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        processMesh(scene->mMeshes[i], scene);
    }
}

// no idea what's going on here
void Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, unsigned int materialIndex) {
    unsigned int textureCount = mat->GetTextureCount(type);
    if (textureCount == 0) return;
    
    aiString path;
    mat->GetTexture(type, 0, &path);
    string texPath = path.C_Str();
    
    for (auto& tex : loadedTextures) {
        if (tex && tex->path == texPath) {
            textures[materialIndex] = tex;
            return;
        }
    }
    
    string fullPath = "models/" + texPath;
    if (access(fullPath.c_str(), F_OK) == -1) {
        cout << "Texture not found: " << texPath << " . Using fallback." << endl;
        fullPath = "models/missing.jpg";
    }
    
    // checks file extension and sets format then creates new texture object then stores the path to it and
    // adds to an array and a vector (loadedTextures)
    try {
        GLenum format = GL_RGBA;
        // most extensions are rgba like png but stuff like jpg/jpeg are rgb
        string ext = fullPath.substr(fullPath.find_last_of(".") + 1);
        for (auto& c : ext) c = tolower(c);
        if (ext == "jpg" || ext == "jpeg") format = GL_RGB;
        
        Texture* texture = new Texture(fullPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, format, GL_UNSIGNED_BYTE);
        texture->path = texPath;
        textures[materialIndex] = texture;
        loadedTextures.push_back(texture);
    } catch (const exception& e) {
        cout << "Failed to load texture: " << fullPath << " - " << e.what() << endl;
    }
}

// or here barely
void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    vector<float> vertices;
    vector<unsigned int> indices;
    
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        
        if (mesh->HasNormals()) {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }
    
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    Mesh newMesh;
    newMesh.indexCount = indices.size();
    newMesh.materialIndex = mesh->mMaterialIndex;
    
    glGenVertexArrays(1, &newMesh.VAO);
    glGenBuffers(1, &newMesh.VBO);
    glGenBuffers(1, &newMesh.EBO);
    
    glBindVertexArray(newMesh.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    
    meshes.push_back(newMesh);
} 